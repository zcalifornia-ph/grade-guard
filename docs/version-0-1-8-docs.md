# Version 0.1.8 Notes

## Title
Unit 4 / Bolt 4.1 Persistence Contract and CSV Compatibility Update for Grade Guard

## Quick Diagnostic Read

You are looking at the correct `v0.1.8` repository state if it now contains:

- the status-based persistence API in `grade-guard/header/persistence.h`,
- the versioned `GRADE_GUARD_CSV,1` writer and legacy compatibility loader in `grade-guard/source/persistence.c`,
- the updated app bootstrap/save flow in `grade-guard/source/app.c`,
- the new persistence regression harness in `grade-guard/unit-tests/persistence_contract_test.c`,
- the Bolt-specific persistence contract note at `docs/unit-4-bolt-4-1-persistence-contract.md`,
- a root `README.md` updated to `v0.1.8`,
- a new top entry in `CHANGELOG.md`,
- and this file at `docs/version-0-1-8-docs.md`.

The planning and governance state changed only where the repository context required it:

- `REQUIREMENTS.md` now records Bolt 4.1 completion evidence and keeps the Unit 4 human schema-approval gate open for the next step.
- `CONTRIBUTING.md` and `README.md` were updated because persistence-touching changes now have one dedicated regression path and the repository status/version text had become stale after the Unit 4 work landed.
- `SECURITY.md` and `CODE_OF_CONDUCT.md` were not changed because this version does not alter vulnerability reporting or community-governance policy.

## One-Sentence Objective

Replace the implicit, fragile CSV persistence behavior with one explicit contract for new saves, preserve compatibility with historical profile files, and document the migration boundary before malformed-file hardening begins.

## Why This Version Matters

`v0.1.7` extracted the runtime app/controller path and wired `persistence.c` into the live build.
It did not yet define a trustworthy storage contract for those runtime calls.

That left the persistence boundary in an unstable state:

- the writer emitted an ad hoc file shape,
- the loader depended on unlabeled row ordering and blank-line sentinels,
- the app still treated file naming/discovery as a local implementation detail,
- and the repository had no focused regression proving round-trip persistence or backward compatibility.

This version matters because it fixes that contract gap:

- `persistence.h` now exposes the actual save/load/list API the app relies on,
- new saves are versioned and row-tagged through `GRADE_GUARD_CSV,1`,
- the loader still accepts the historical unversioned format used by existing local profiles,
- and the repository now has targeted tests for lecture-only, lab, mixed, and legacy fixture scenarios.

What this version does not do is equally important:

- it does not yet harden every malformed/truncated input path,
- it does not claim arbitrary hand-edited CSV compatibility,
- and it does not close the broader Unit 4 schema-approval gate that should still be reviewed before Bolt 4.2 is considered finished.

## What Changed (Artifact Map)

- Persistence API:
  - `grade-guard/header/persistence.h`
- Persistence implementation:
  - `grade-guard/source/persistence.c`
- Runtime caller updated to consume the persistence boundary:
  - `grade-guard/source/app.c`
- New focused regression:
  - `grade-guard/unit-tests/persistence_contract_test.c`
- Bolt-specific design and evidence note:
  - `docs/unit-4-bolt-4-1-persistence-contract.md`
- Updated release and contributor docs:
  - `README.md`
  - `CHANGELOG.md`
  - `CONTRIBUTING.md`
  - `REQUIREMENTS.md`

## Guided Walkthrough

## 1) The Persistence Boundary Is Finally Real

Before this version, the persistence module existed physically, but its contract was still weak:

- save/load calls had no status return,
- file naming leaked into the app layer,
- and the schema lived only in implementation side effects.

`v0.1.8` fixes that by publishing one real boundary:

- `persistence_save_student_profile(...)`
- `persistence_load_student_profile(...)`
- `persistence_build_profile_path(...)`
- `persistence_list_profile_file_paths(...)`
- `persistence_destroy_profile_file_paths(...)`

That means the runtime app no longer has to invent its own persistence behavior.

## 2) New Saves Now Have an Explicit CSV Grammar

The new writer emits:

```text
GRADE_GUARD_CSV,1
PROFILE,...
COURSE,...
PARAMETER,lecture,...
ACTIVITY,...
```

That matters because the repository now has:

- one version header,
- one required profile record,
- tagged course/parameter/activity rows,
- explicit lecture versus lab component rows,
- and one documented ordering rule.

The format is still simple on purpose.
Bolt 4.1 chooses stability and traceability first, not a fully general CSV parser.

## 3) Old Numbered Profile Files Still Load

The repository already contained historical files like `0.csv`.
Breaking those immediately would make the new persistence boundary unusable in practice.

This version therefore keeps a compatibility reader for the old format:

- legacy profile header row still loads,
- `Yes` / `No` course lab flags still load,
- `N/A` text placeholders normalize to empty strings,
- and the historical `0.00` parameter-percentage convention is used as the safe boundary for disambiguating old unlabeled rows.

That is a compatibility decision, not a claim that every old or hand-edited CSV variant is supported.

## 4) `app.c` Now Treats Persistence as a Service

`app.c` no longer:

- probes numbered CSV files by itself,
- constructs file paths directly,
- or silently assumes save/load always succeeds.

Instead it now:

- asks `persistence_list_profile_file_paths(".")` for the current repository list,
- loads each profile through the persistence API,
- saves each profile through the same boundary,
- and surfaces load/save failures through explicit UI failure messages.

That keeps the startup/shutdown orchestration smaller and makes later persistence hardening local to the persistence module.

## 5) There Is Now a Real Regression for the Contract

`grade-guard/unit-tests/persistence_contract_test.c` adds four kinds of evidence:

- round-trip persistence for a lecture-only profile,
- round-trip persistence for a lab-bearing profile,
- round-trip persistence for a mixed multi-course profile,
- and legacy-load coverage for the historical unversioned file shape.

It also verifies numbered-file discovery through a temporary runtime output directory.

That is the key difference between "the code seems reasonable" and "the repository has repeatable proof for the contract."

## 6) The Remaining Gap Is Now Clearer

This version intentionally leaves Bolt 4.2 for the next cycle.

What still needs hardening next:

- malformed or truncated row handling beyond the current safe-fail baseline,
- clearer user-visible recovery behavior for bad files,
- a more explicit statement of unsupported edge cases if any remain after that,
- and final human approval of the schema and compatibility approach.

That is a healthier place to be than the pre-Bolt-4.1 state, because the repository now knows what the intended contract actually is.

## Validation Evidence

- Acceptance build (2026-03-13): `gcc -std=c17 -Wall -Wextra -pedantic -I grade-guard/header grade-guard/main.c grade-guard/source/app.c grade-guard/source/vector.c grade-guard/source/models.c grade-guard/source/grade_calc.c grade-guard/source/persistence.c grade-guard/source/ui_console.c grade-guard/source/profile_controller.c -o grade-guard/bolt-4-1-main.exe` succeeded.
- Persistence contract regression compile and run (2026-03-13): `gcc -std=c17 -Wall -Wextra -pedantic -I grade-guard/header grade-guard/unit-tests/persistence_contract_test.c grade-guard/source/persistence.c grade-guard/source/models.c grade-guard/source/vector.c -o grade-guard/unit-tests/persistence_contract_test.exe` succeeded, and `.\grade-guard\unit-tests\persistence_contract_test.exe` printed `persistence contract tests passed`.
- Vector regression run (2026-03-13): `.\grade-guard\tests\vector_test.exe` printed `vector tests passed`.
- Lifecycle regression run (2026-03-13): `.\grade-guard\unit-tests\models_lifecycle_test.exe` printed `models lifecycle tests passed`.
- Unit 4 schema approval (2026-03-13): not completed in this session, so the human validation line remains open in `REQUIREMENTS.md`.

## Copy-Paste Commands

### Read the Bolt 4.1 contract note

```powershell
Get-Content docs/unit-4-bolt-4-1-persistence-contract.md
```

### Rebuild the app with the persistence contract update

```powershell
gcc -std=c17 -Wall -Wextra -pedantic -I grade-guard/header grade-guard/main.c grade-guard/source/app.c grade-guard/source/vector.c grade-guard/source/models.c grade-guard/source/grade_calc.c grade-guard/source/persistence.c grade-guard/source/ui_console.c grade-guard/source/profile_controller.c -o grade-guard/bolt-4-1-main.exe
```

### Re-run the focused persistence regression

```powershell
gcc -std=c17 -Wall -Wextra -pedantic -I grade-guard/header grade-guard/unit-tests/persistence_contract_test.c grade-guard/source/persistence.c grade-guard/source/models.c grade-guard/source/vector.c -o grade-guard/unit-tests/persistence_contract_test.exe
.\grade-guard\unit-tests\persistence_contract_test.exe
```

## Pitfalls + Debugging

### 1) The app builds but profiles no longer load

- Cause: the saved file may not match either the documented v1 schema or the historical compatibility shape.
- Fix: inspect the file against `docs/unit-4-bolt-4-1-persistence-contract.md` before changing the parser.

### 2) A save call fails on otherwise valid profile data

- Cause: Bolt 4.1 intentionally rejects commas and embedded newlines in persisted text fields because quoting/escaping support is not part of this version.
- Fix: either normalize that text before save or extend the contract deliberately in a later Bolt.

### 3) It is tempting to treat legacy compatibility as "all old CSVs are supported"

- Do not do that.
- The compatibility path is designed for the historical repository files produced by the previous app behavior, not for arbitrary external CSV formats.

## Immediate Next Actions

1. Review and approve the Unit 4 schema and compatibility approach with the new contract note as the baseline.
2. Implement Bolt 4.2 malformed/truncated CSV hardening on top of the now-explicit persistence boundary.
3. Extend the persistence regression with negative malformed-file cases once Bolt 4.2 defines the final failure semantics.
4. Keep the app/save/load/UI boundary changes small so later grade-engine work does not re-couple persistence into unrelated modules.
