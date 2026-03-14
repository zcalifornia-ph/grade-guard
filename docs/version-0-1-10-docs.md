# Version 0.1.10 Notes

## Title
Persistence Parsing/Serialization Hardening and User-Visible Failure Mapping for Grade Guard

## Quick Diagnostic Read

You are looking at the correct `v0.1.10` repository state if it now contains:

- a root `README.md` updated to `v0.1.10`,
- a new top `v0.1.10` entry in `CHANGELOG.md`,
- this file at `docs/version-0-1-10-docs.md`,
- `docs/unit-4-bolt-4-2-persistence-hardening.md`,
- a hardened `grade-guard/source/persistence.c` that rejects oversized logical records safely,
- an updated `grade-guard/source/app.c` that surfaces persistence failures with status-specific user messages,
- and an expanded `grade-guard/unit-tests/persistence_contract_test.c` that now covers both round-trip and malformed-input scenarios.

The project-governance documents stayed unchanged for a reason:

- `CONTRIBUTING.md` was not changed because the existing focused build/test workflow already covered this release's validation path.
- `SECURITY.md` was not changed because this release hardens local CSV parsing behavior, not vulnerability-reporting policy.
- `CODE_OF_CONDUCT.md` was not changed because this release does not alter community-governance expectations.

## One-Sentence Objective

Make the persistence layer reject malformed, truncated, and oversized CSV data safely while giving the runtime app clearer failure reporting and preserving the existing versioned-plus-legacy compatibility contract.

## Why This Version Matters

`v0.1.9` improved repository hygiene and file-level attribution.
It did not yet close the next functional gap inside Unit 4: the persistence layer still depended on a fixed line buffer without treating oversized logical records as an explicit contract boundary, and the app still collapsed every persistence problem into a generic failure message.

That left three practical problems:

- a malformed or oversized CSV row could be split across reads in ways that were not documented as a first-class failure mode,
- the save path could still emit rows longer than the fixed reader contract if the text stayed comma-free,
- and the runtime user could not tell whether a load/save failure was caused by malformed data, memory pressure, or plain file I/O trouble.

This version matters because it closes those gaps without changing the versioned CSV grammar itself:

- the loader now treats the fixed line buffer as a hard record boundary and flushes oversized rows before failing,
- the writer now refuses to serialize records that the reader would not be able to consume safely,
- the app now maps persistence statuses to clearer load/save failure messages,
- and the persistence regression harness now proves the negative paths instead of only the happy-path round trip.

What this version does not do is also important:

- it does not change the `GRADE_GUARD_CSV,1` schema version,
- it does not remove legacy-load compatibility,
- it does not close the Unit 4 human-validation gate,
- and it does not add controller-level automation for the live Windows console flow.

## What Changed (Artifact Map)

- Runtime persistence hardening:
  - `grade-guard/source/persistence.c`
- Runtime failure-message mapping:
  - `grade-guard/source/app.c`
- Persistence regression expansion:
  - `grade-guard/unit-tests/persistence_contract_test.c`
- Bolt-specific documentation:
  - `docs/unit-4-bolt-4-2-persistence-hardening.md`
- Release metadata:
  - `README.md`
  - `CHANGELOG.md`
  - `docs/version-0-1-10-docs.md`

## Guided Walkthrough

## 1) Oversized Logical Records Are Now Explicitly Rejected

The core implementation change in `grade-guard/source/persistence.c` is that CSV loading no longer relies on raw `fgets()` calls alone.
Instead, the loader now reads through an overflow-aware helper that:

- detects when a logical row does not fit inside the fixed persistence line buffer,
- discards the remainder of that oversized row,
- and returns a parse failure before any tail fragment can be misread as a new record.

That turns an implicit parser weakness into an explicit, testable contract.

## 2) The Writer Now Honors the Same Record-Size Contract

This release also closes the save-side mismatch.
Before `v0.1.10`, a text field could be comma-free and newline-free yet still produce a row longer than the fixed read buffer.

The writer now validates serialized record length for:

- `PROFILE` rows,
- `COURSE` rows,
- `PARAMETER` rows,
- and `ACTIVITY` rows.

If any row would exceed the supported logical-line size, save now fails early with `PERSISTENCE_STATUS_UNSUPPORTED_TEXT` instead of emitting a file the reader cannot safely consume.

## 3) Persistence Failures Are More Visible to the User

`grade-guard/source/app.c` now maps persistence status codes to more specific runtime messages.
That means the app can distinguish:

- malformed/truncated/oversized saved data,
- invalid in-memory profile state,
- file I/O problems,
- and memory allocation failures.

This is a practical reliability improvement even though it does not change the menu structure or persistence file format.

## 4) The Persistence Harness Now Covers Negative Paths

`grade-guard/unit-tests/persistence_contract_test.c` still covers lecture-only, lab, mixed-profile, legacy-load, and numbered-file-listing behavior.
`v0.1.10` extends that same harness with focused negative coverage for:

- blank lines,
- missing fields,
- invalid numeric tokens after partial parse progress,
- oversized load records,
- and oversized save rejection.

That is the core evidence that the new parser/writer contract is enforced rather than only documented.

## 5) Manual Cleanup Still Matters Before a Human Commit

As with prior releases, this task does not delete generated binaries, CSV fixtures, or build directories automatically.
Instead, `CHANGELOG.md` now marks the current local artifacts under `### For Deletion` so they can be removed manually if they are not meant to remain in the working tree.

## Validation Evidence

- Acceptance build (2026-03-13):
  - `gcc -std=c17 -Wall -Wextra -pedantic -I grade-guard/header grade-guard/main.c grade-guard/source/app.c grade-guard/source/vector.c grade-guard/source/models.c grade-guard/source/grade_calc.c grade-guard/source/persistence.c grade-guard/source/ui_console.c grade-guard/source/profile_controller.c -o grade-guard/bolt-4-2-main.exe`
- Persistence contract regression compile (2026-03-13):
  - `gcc -std=c17 -Wall -Wextra -pedantic -I grade-guard/header grade-guard/unit-tests/persistence_contract_test.c grade-guard/source/persistence.c grade-guard/source/models.c grade-guard/source/vector.c -o grade-guard/unit-tests/persistence_contract_test.exe`
- Persistence contract regression run (2026-03-13):
  - `.\grade-guard\unit-tests\persistence_contract_test.exe`
  - Result: `persistence contract tests passed`
- Vector regression run (2026-03-13):
  - `.\grade-guard\tests\vector_test.exe`
  - Result: `vector tests passed`
- Models regression run (2026-03-13):
  - `.\grade-guard\unit-tests\models_lifecycle_test.exe`
  - Result: `models lifecycle tests passed`

## Copy-Paste Commands

### Read this release note

```powershell
Get-Content docs/version-0-1-10-docs.md
```

### Rebuild the runtime acceptance target

```powershell
gcc -std=c17 -Wall -Wextra -pedantic -I grade-guard/header grade-guard/main.c grade-guard/source/app.c grade-guard/source/vector.c grade-guard/source/models.c grade-guard/source/grade_calc.c grade-guard/source/persistence.c grade-guard/source/ui_console.c grade-guard/source/profile_controller.c -o grade-guard/bolt-4-2-main.exe
```

### Re-run the persistence regression harness

```powershell
gcc -std=c17 -Wall -Wextra -pedantic -I grade-guard/header grade-guard/unit-tests/persistence_contract_test.c grade-guard/source/persistence.c grade-guard/source/models.c grade-guard/source/vector.c -o grade-guard/unit-tests/persistence_contract_test.exe
.\grade-guard\unit-tests\persistence_contract_test.exe
```

## Pitfalls + Debugging

### 1) Do not treat blank-line tolerance as general malformed-file forgiveness

- Empty lines are intentionally ignored.
- Missing fields, invalid numeric tokens, and oversized rows are still hard failures.

### 2) Do not forget that save-side rejection is part of the contract now

- A text value can be comma-free and still be unsupported if the serialized row would exceed the fixed reader boundary.
- If save suddenly fails for a long course, parameter, or activity label, that is now expected behavior.

### 3) Do not assume the Unit 4 review gate is closed

- This version lands the design, implementation, test, and documentation work for Bolt 4.2.
- Human validation of the schema/compatibility approach and the live end-user failure-path wording still remains open.

### 4) Do not forget the generated artifacts before a manual commit

- This task intentionally left `.exe` outputs, generated CSV fixtures, and the smoke-build directory untouched.
- Review the new `CHANGELOG.md` `For Deletion` section and remove anything that should not stay in the repository.

## Immediate Next Actions

1. Manually delete the generated binaries, CSV fixtures, and build directories listed in `CHANGELOG.md` if they are not meant to be committed.
2. Run one live Windows console pass to confirm the new persistence failure messages are clear and appropriate from the user's perspective.
3. Close the open Unit 4 schema/review gate only after that human validation is done.
4. Continue the next functional work on grade-engine defects and remaining validation gaps rather than reopening the persistence contract itself unless new evidence appears.
