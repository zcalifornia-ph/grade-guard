# Unit 4 Bolt 4.2: Persistence Hardening

Objective: harden CSV load/save behavior so malformed files fail safely, oversized records never slip past the parser contract, and persistence failures surface as specific user-visible messages.

## Executable Contract

Goal / deliverable

- Reject malformed persistence rows deterministically instead of letting truncated reads spill into later records.
- Refuse to serialize rows that exceed the persistence reader's fixed logical-line contract.
- Preserve the existing versioned-v1 plus legacy-load compatibility boundary from Bolt 4.1.
- Surface distinct load/save failure causes through the app layer instead of one generic persistence failure message.

Bolt checklist

- Design: define handling for missing fields, invalid numbers, empty lines, and oversized records.
- Implement: harden the reader/writer and the app-facing persistence error path.
- Test: extend the persistence contract suite with negative fixtures and save-side rejection coverage.
- Docs: record the failure matrix and the remaining unsupported cases.
- Review: verify the failure states are visible and that failed loads do not leave partial profile state behind.

Acceptance criteria interpreted for this Bolt

- V1 and legacy loaders reject malformed or oversized rows without crashing and without preserving partial loaded state.
- Empty lines remain ignored as benign formatting noise.
- New saves still honor the explicit `GRADE_GUARD_CSV,1` schema and are rejected before write if a row would exceed the supported record size.
- The runtime app can distinguish malformed/truncated CSV, IO failures, invalid in-memory profile state, and unsupported save text when reporting failures to the user.

Dependencies / interfaces touched

- Upstream contract: `REQUIREMENTS.md`
- Runtime caller: `grade-guard/source/app.c`
- Persistence boundary: `grade-guard/header/persistence.h` and `grade-guard/source/persistence.c`
- Domain lifecycle helpers: `grade-guard/header/models.h` and `grade-guard/source/models.c`
- Verification: `grade-guard/unit-tests/persistence_contract_test.c`

Relevant NFRs and risks for this Bolt

- NFR-01 matters because malformed CSV must fail without out-of-bounds access or corrupted parsing state.
- NFR-04 matters because the hardened persistence path still has to build cleanly under the project warning flags.
- NFR-05 matters because failed loads must leave no half-owned nested vectors behind.
- The main brownfield risk was the fixed line buffer: Bolt 4.1 introduced a formal schema, but Bolt 4.2 had to ensure the writer never emits rows the reader cannot consume safely.

## Context Summary

- `persistence.c` already enforced exact field counts and numeric parsing, but it still relied on `fgets()` directly without detecting logical lines longer than the fixed buffer.
- An oversized CSV row could therefore be split across multiple parser iterations, which risked confusing later records even if the final status became a parse error.
- `app.c` still collapsed every load or save failure into a generic message, so the persistence layer could distinguish failures internally without giving the user actionable feedback.
- Failed loads already reset the top-level `Student_Profile`, which made it feasible to turn malformed-file handling into an explicit contract and prove it with tests.
- Bolt 4.1's unsupported text rule still rejected commas and embedded newlines on save, but it did not yet reject rows that were plain-text safe while still longer than the reader contract.

## ADR Bullets

- Treat the persistence line buffer as a hard contract: any logical row longer than 1023 characters is malformed on load and unsupported on save.
- Keep empty lines as ignorable formatting noise for both the versioned and legacy loaders.
- Flush the remainder of an oversized row before returning a parse failure so the loader never reinterprets the tail of one record as a separate record.
- Validate the serialized length of each profile, course, parameter, and activity row before writing any file content.
- Surface persistence status codes through `app.c` so the user sees whether the failure was IO, malformed data, memory pressure, or unsupported text.

## Failure Matrix

| Case | Load behavior | Save behavior |
| --- | --- | --- |
| Empty line | Ignored | Writer does not emit empty lines |
| Missing required field | `PERSISTENCE_STATUS_PARSE_ERROR`; loaded profile is reset | Not applicable; writer always emits required fields from in-memory state |
| Invalid numeric token | `PERSISTENCE_STATUS_PARSE_ERROR`; loaded profile is reset | Not applicable; numbers come from typed in-memory fields |
| Oversized logical row (>1023 chars) | `PERSISTENCE_STATUS_PARSE_ERROR`; oversized remainder is flushed and loaded profile is reset | `PERSISTENCE_STATUS_UNSUPPORTED_TEXT` before file write |
| Comma/newline in saved text | Split or malformed input remains a parse failure | `PERSISTENCE_STATUS_UNSUPPORTED_TEXT` before file write |
| Invalid in-memory nested structure | Not applicable | `PERSISTENCE_STATUS_INVALID_ARGUMENT` before file write |

## Validation Evidence

- Acceptance build (2026-03-13): `gcc -std=c17 -Wall -Wextra -pedantic -I grade-guard/header grade-guard/main.c grade-guard/source/app.c grade-guard/source/vector.c grade-guard/source/models.c grade-guard/source/grade_calc.c grade-guard/source/persistence.c grade-guard/source/ui_console.c grade-guard/source/profile_controller.c -o grade-guard/bolt-4-2-main.exe` succeeded.
- Persistence contract regression compile (2026-03-13): `gcc -std=c17 -Wall -Wextra -pedantic -I grade-guard/header grade-guard/unit-tests/persistence_contract_test.c grade-guard/source/persistence.c grade-guard/source/models.c grade-guard/source/vector.c -o grade-guard/unit-tests/persistence_contract_test.exe` succeeded.
- Persistence contract regression run (2026-03-13): `.\grade-guard\unit-tests\persistence_contract_test.exe` exited `0` and printed `persistence contract tests passed`.
- Extended negative coverage (2026-03-13): the same persistence contract run now exercises blank-line tolerance, missing fields, invalid numeric tokens after partially parsed state, oversized load records, and oversized save rejection.
- Vector regression run (2026-03-13): `.\grade-guard\tests\vector_test.exe` exited `0` and printed `vector tests passed`.
- Models regression run (2026-03-13): `.\grade-guard\unit-tests\models_lifecycle_test.exe` exited `0` and printed `models lifecycle tests passed`.

## Review Status

- Code-level review confirms failed loads still zero the profile state and that `app.c` now maps persistence statuses to specific user-visible failure messages.
- Live human validation of the Unit 4 schema/compatibility approach and the end-user failure-path wording remains open before Bolt 4.2 itself should be checked off.
