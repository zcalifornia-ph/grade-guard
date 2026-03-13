# Unit 4 Bolt 4.1: Persistence Contract

Objective: replace the ad hoc CSV coupling with one explicit persistence contract, keep the runtime app on a dedicated save/load/list API, and preserve compatibility with historical numbered profile files.

## Executable Contract

Goal / deliverable

- Publish one versioned CSV contract for new saves.
- Keep the runtime entry path on `grade-guard/header/persistence.h` and `grade-guard/source/persistence.c`.
- Preserve load compatibility with legacy pre-Bolt-4.1 files already written by the app.
- Add focused regression coverage for representative lecture-only, lab, and mixed profile shapes.

Bolt checklist

- Design: document the schema, ordering, delimiters, required rows, and compatibility stance.
- Implement: move callers to the dedicated persistence save/load/list API with status returns.
- Test: prove round-trip behavior on at least three profile shapes, plus one legacy fixture.
- Docs: record migration rules, unsupported text cases, and repository file-numbering behavior.
- Review: confirm the contract is stable enough for both new saves and old repository files.

Acceptance criteria interpreted for this Bolt

- The persistence module owns a versioned file grammar instead of relying on unlabeled blank-line sentinels.
- `app.c` uses the dedicated persistence boundary for save, load, and numbered-profile discovery.
- Representative profile shapes round-trip through the persisted schema without losing profile, course, parameter, or activity data.
- Legacy files from the historical unversioned format still load, and the compatibility limits are explicit.

Dependencies / interfaces touched

- Upstream contract: `REQUIREMENTS.md`
- Domain ownership rules: `grade-guard/header/models.h` and `grade-guard/source/models.c`
- Runtime caller: `grade-guard/source/app.c`
- Persistence boundary: `grade-guard/header/persistence.h` and `grade-guard/source/persistence.c`
- Verification: `grade-guard/unit-tests/persistence_contract_test.c`

Relevant NFRs and risks for this Bolt

- NFR-04 matters because the runtime save/load caller now depends on a status-based persistence interface instead of implicit file helpers.
- NFR-05 matters because malformed or ambiguous historical rows must fail cleanly instead of corrupting nested vectors.
- The main brownfield risk was legacy compatibility: historical files do not tag row types, so the compatibility reader has to be explicit about the assumptions it can safely honor.

## Context Summary

- The old writer and loader did not share a reliable grammar; the loader depended on separators the writer never emitted.
- `app.c` previously built file names itself and treated CSV discovery as a low-level loop instead of a persistence concern.
- The runtime still stores one profile per numbered CSV file in the working directory, so file discovery remains part of the persistence boundary.
- `Student_Profile`, `Course`, `Course_Parameter`, and `Activities` are all stored by value in vectors, so load paths must transfer ownership carefully after each successful push.
- `parameter->percentage` is still part of the persisted contract even though the runtime does not currently calculate it, because Bolt 4.1 focuses on preserving stored data, not pruning fields.
- Historical files written before this Bolt use `Yes`/`No` course rows and unlabeled 3-field parameter/activity rows, which makes compatibility heuristic-driven by the old `0.00` parameter percentage convention.

## ADR Bullets

- Adopt a versioned row-oriented schema for all new saves: `GRADE_GUARD_CSV,1`.
- Keep numbered file naming (`0.csv`, `1.csv`, ...) as the repository contract for now, but move path creation and contiguous discovery behind the persistence module.
- Support legacy load but not legacy save: once an old file is loaded and saved again, the writer rewrites it into the new versioned contract.
- Do not implement CSV quoting in Bolt 4.1; commas and embedded newlines remain unsupported text for saved fields and are rejected before write.

## CSV Contract

Version header

- First line must be `GRADE_GUARD_CSV,1`.

Row ordering

- Exactly one `PROFILE` row must appear before any `COURSE` rows.
- Each `COURSE` row may be followed by zero or more `PARAMETER,lecture,...` rows.
- If the course has `lab_flag = 1`, it may also be followed by zero or more `PARAMETER,lab,...` rows.
- Each `PARAMETER` row may be followed by zero or more `ACTIVITY` rows.
- Blank lines are ignored by the v1 loader but are not emitted by the writer.

Delimiter and text rules

- The delimiter is a literal comma.
- New saves do not support commas, carriage returns, or line feeds inside text fields.
- Empty text is represented as an empty field, not as the old `N/A` sentinel.

Row definitions

| Row type | Fields |
| --- | --- |
| `PROFILE` | `PROFILE,first_name,middle_name,last_name,student_number,degree_program,predicted_gwa,goal` |
| `COURSE` | `COURSE,name,units,lab_flag` where `lab_flag` is `0` or `1` |
| `PARAMETER` | `PARAMETER,lecture|lab,parameter_name,weight,percentage` |
| `ACTIVITY` | `ACTIVITY,activity_name,score,total_score` |

Repository file listing contract

- The runtime repository is still the current working directory.
- Managed files are named `<index>.csv` with zero-based contiguous numbering.
- `persistence_list_profile_file_paths()` probes `0.csv`, `1.csv`, and so on until the first missing index.

## Compatibility Rules

Legacy load support

- The loader still accepts historical unversioned files that begin with the old profile header row.
- Legacy course rows still use `Yes` and `No` for the lab flag.
- Legacy `N/A` text placeholders are normalized to empty strings on load.

Legacy limitations

- Historical unlabeled 3-field rows are interpreted using the old convention that parameter rows carried a `0.00` percentage field.
- That compatibility path is intended for the repository files emitted by the pre-Bolt-4.1 app, not for arbitrary hand-edited CSV variants.
- Unsupported or ambiguous legacy rows still fail the load rather than guessing and corrupting state.

Migration behavior

- Load a legacy file: accepted through the compatibility reader.
- Save that loaded profile again: emitted as `GRADE_GUARD_CSV,1`.
- No in-place file deletion or cleanup is performed by this Bolt.

## Validation Evidence

- Acceptance build (2026-03-13): `gcc -std=c17 -Wall -Wextra -pedantic -I grade-guard/header grade-guard/main.c grade-guard/source/app.c grade-guard/source/vector.c grade-guard/source/models.c grade-guard/source/grade_calc.c grade-guard/source/persistence.c grade-guard/source/ui_console.c grade-guard/source/profile_controller.c -o grade-guard/bolt-4-1-main.exe` succeeded.
- Persistence contract regression compile (2026-03-13): `gcc -std=c17 -Wall -Wextra -pedantic -I grade-guard/header grade-guard/unit-tests/persistence_contract_test.c grade-guard/source/persistence.c grade-guard/source/models.c grade-guard/source/vector.c -o grade-guard/unit-tests/persistence_contract_test.exe` succeeded.
- Persistence contract regression run (2026-03-13): `.\grade-guard/unit-tests/persistence_contract_test.exe` exited `0` and printed `persistence contract tests passed`.
- Vector regression run (2026-03-13): `.\grade-guard/tests/vector_test.exe` exited `0` and printed `vector tests passed`.
- Models regression run (2026-03-13): `.\grade-guard/unit-tests/models_lifecycle_test.exe` exited `0` and printed `models lifecycle tests passed`.

## Review Status

- Reviewed against both sides of the migration boundary: new files now have explicit row tags and versioning, while historical numbered files still load through the documented compatibility path.
- Unit 4 human validation for the schema and compatibility approach remains open until the broader malformed-file hardening work in Bolt 4.2 is reviewed.
