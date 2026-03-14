# Changelog

Status: source-backed Windows console grade-tracking prototype with extracted utility, domain lifecycle, grade-engine, UI/platform, app orchestration, workflow-controller, hardened versioned persistence modules, repository-wide source attribution coverage, and validated Unit 5 defect fixes; remaining live Windows workflow review and Unit 6 release-readiness work are still pending.

## v0.1.12 - 2026-03-14

### Added or Changed
- Updated `README.md` from `v0.1.11` to `v0.1.12`, refreshed the top-level project status/progress snapshot for completed Unit 5 remediation, documented the post-fix Unit 5 regression command, and pointed the release trail at the new `v0.1.12` notes plus the Unit 5 before/after bolt docs.
- Added `docs/version-0-1-12-docs.md` with the fuller explanation of the Unit 5 fix scope, validation evidence, remaining manual-review gap, and cleanup guidance for generated artifacts.
- Added `docs/unit-5-bolt-5-2-defect-fixes.md` to document the Bolt 5.2 contract, ADR bullets, final defect outcomes, regression coverage, and review status.
- Updated `grade-guard/header/grade_calc.h` and `grade-guard/source/grade_calc.c` so zero-score activities count toward weighted averages, lab-course weighting stays normalized, over-total scores are bounded safely during aggregation, and predicted GWA rendering can distinguish no-data profiles through `calculate_predicted_gwa()`.
- Updated `grade-guard/header/ui_console.h`, `grade-guard/source/ui_console.c`, and `grade-guard/source/profile_controller.c` so shared selection handling returns an explicit cancel status on `Esc`, affected controller screens back out cleanly on cancel, and empty/zero-unit profiles render `Predicted GWA: N/A` instead of a fabricated `5.00`.
- Updated `grade-guard/unit-tests/unit5_defect_baseline_test.c` from a baseline defect-reproduction harness into a fixed-behavior regression suite covering zero-score inclusion, normalized lab weighting, bounded over-total scores, zero-course and zero-unit no-data handling, valid all-zero-coursework `5.00` rendering, and shared `Esc` cancellation.
- Kept `CONTRIBUTING.md`, `SECURITY.md`, and `CODE_OF_CONDUCT.md` unchanged because this version changes runtime behavior and release documentation, not contributor workflow policy, vulnerability-reporting policy, or community-governance rules.

### For Deletion
- `grade-guard.exe` (root-level local binary artifact present in the workspace).
- `grade-guard/build-smoke/` (smoke-build output directory still present in the workspace).
- `grade-guard/0.csv` (numbered profile output currently present in the working directory).
- `grade-guard/bolt-4-1-main.exe` (local acceptance-build artifact from the prior Bolt 4.1 validation run).
- `grade-guard/bolt-4-2-main.exe` (local acceptance-build artifact generated while validating Bolt 4.2).
- `grade-guard/bolt-5-1-main.exe` (local acceptance-build artifact generated while validating Bolt 5.1).
- `grade-guard/bolt-5-2-main.exe` (local acceptance-build artifact generated while validating Bolt 5.2).
- `grade-guard/tests/vector_test.exe` (local vector regression-test binary present in the workspace).
- `grade-guard/unit-tests/models_lifecycle_test.exe` (local lifecycle regression-test binary present in the workspace).
- `grade-guard/unit-tests/persistence_contract_test.exe` (local persistence-contract regression binary present in the workspace).
- `grade-guard/unit-tests/unit5_defect_baseline_test.exe` (local Unit 5 defect-regression binary present in the workspace).
- `grade-guard/unit-tests/persistence-blank-lines.csv` (generated blank-line fixture output from the persistence regression).
- `grade-guard/unit-tests/persistence-invalid-number.csv` (generated invalid-number fixture output from the persistence regression).
- `grade-guard/unit-tests/persistence-legacy-fixture.csv` (generated legacy compatibility fixture output from the persistence regression).
- `grade-guard/unit-tests/persistence-missing-field.csv` (generated missing-field fixture output from the persistence regression).
- `grade-guard/unit-tests/persistence-oversized-record.csv` (generated oversized-record fixture output from the persistence regression).
- `grade-guard/unit-tests/persistence-shape-lecture.csv` (generated round-trip lecture-only output from the persistence regression).
- `grade-guard/unit-tests/persistence-shape-lab.csv` (generated round-trip lab-course output from the persistence regression).
- `grade-guard/unit-tests/persistence-shape-mixed.csv` (generated round-trip mixed-profile output from the persistence regression).
- `grade-guard/unit-tests/persistence-runtime/` (generated numbered runtime-output directory from the persistence regression and currently containing `0.csv` and `1.csv`).

## v0.1.11 - 2026-03-14

### Added or Changed
- Updated `README.md` from `v0.1.10` to `v0.1.11`, refreshed the project status/progress snapshot for Unit 5, documented the new defect-baseline harness command, and pointed the release trail at the `v0.1.11` notes plus the new Unit 5 Bolt 5.1 register.
- Added `grade-guard/unit-tests/unit5_defect_baseline_test.c` to reproduce the current baseline defects around zero-score handling, lab-course weighting, over-total score acceptance, empty-profile GWA rendering, and shared selection-handler `Esc` behavior before Bolt 5.2 fixes them.
- Added `docs/unit-5-bolt-5-1-defect-baseline.md` to record the defect triage format, severity rubric, confirmed defect register, proposed first-release fix set, and validation evidence for Unit 5 / Bolt 5.1.
- Added `docs/version-0-1-11-docs.md` with the fuller explanation of this release, the new baseline harness, the defect inventory, and the remaining Unit 5 approval gate.
- Updated `REQUIREMENTS.md` so Unit 5 / Bolt 5.1 now records completed design, implementation, test, and docs subtasks with dated evidence while keeping the review checkbox open until the Bolt 5.2 must-fix set is explicitly approved.
- Kept `CONTRIBUTING.md`, `SECURITY.md`, and `CODE_OF_CONDUCT.md` unchanged because this version adds baseline defect evidence and release documentation, not contributor workflow policy, security-reporting policy, or community-governance rules.

### For Deletion
- `grade-guard.exe` (root-level local binary artifact present in the workspace).
- `grade-guard/build-smoke/` (smoke-build output directory still present in the workspace).
- `grade-guard/0.csv` (numbered profile output currently present in the working directory).
- `grade-guard/bolt-4-1-main.exe` (local acceptance-build artifact from the prior Bolt 4.1 validation run).
- `grade-guard/bolt-4-2-main.exe` (local acceptance-build artifact generated while validating Bolt 4.2).
- `grade-guard/bolt-5-1-main.exe` (local acceptance-build artifact generated while validating Bolt 5.1).
- `grade-guard/tests/vector_test.exe` (local vector regression-test binary present in the workspace).
- `grade-guard/unit-tests/models_lifecycle_test.exe` (local lifecycle regression-test binary present in the workspace).
- `grade-guard/unit-tests/persistence_contract_test.exe` (local persistence-contract regression binary present in the workspace).
- `grade-guard/unit-tests/unit5_defect_baseline_test.exe` (local Unit 5 defect-baseline regression binary present in the workspace).
- `grade-guard/unit-tests/persistence-blank-lines.csv` (generated blank-line fixture output from the persistence regression).
- `grade-guard/unit-tests/persistence-invalid-number.csv` (generated invalid-number fixture output from the persistence regression).
- `grade-guard/unit-tests/persistence-legacy-fixture.csv` (generated legacy compatibility fixture output from the persistence regression).
- `grade-guard/unit-tests/persistence-missing-field.csv` (generated missing-field fixture output from the persistence regression).
- `grade-guard/unit-tests/persistence-oversized-record.csv` (generated oversized-record fixture output from the persistence regression).
- `grade-guard/unit-tests/persistence-shape-lecture.csv` (generated round-trip lecture-only output from the persistence regression).
- `grade-guard/unit-tests/persistence-shape-lab.csv` (generated round-trip lab-course output from the persistence regression).
- `grade-guard/unit-tests/persistence-shape-mixed.csv` (generated round-trip mixed-profile output from the persistence regression).
- `grade-guard/unit-tests/persistence-runtime/` (generated numbered runtime-output directory from the persistence regression and currently containing `0.csv` and `1.csv`).

## v0.1.10 - 2026-03-13

### Added or Changed
- Updated `README.md` from `v0.1.9` to `v0.1.10`, refreshed the top-level repository status/progress snapshot, documented the overflow-aware persistence-hardening behaviors, and pointed the release trail at the new `v0.1.10` notes plus the Bolt 4.2 boundary note.
- Hardened `grade-guard/source/persistence.c` so CSV loads go through an overflow-aware record reader, oversized logical rows are rejected and flushed safely, and save-side serialization rejects profile, course, parameter, and activity rows that exceed the supported parser contract.
- Updated `grade-guard/source/app.c` so persistence load/save failures now map to specific user-visible messages instead of collapsing into one generic failure path.
- Expanded `grade-guard/unit-tests/persistence_contract_test.c` with negative coverage for blank lines, missing fields, invalid numeric tokens after partially parsed state, oversized load records, and oversized save rejection while keeping the existing round-trip and legacy compatibility checks in the same harness.
- Added `docs/unit-4-bolt-4-2-persistence-hardening.md` to document the Bolt 4.2 error-handling matrix, record-size contract, validation evidence, and remaining review gate.
- Added `docs/version-0-1-10-docs.md` with the fuller explanation of this release, validation commands, user-visible persistence behavior, and manual cleanup guidance for generated artifacts.
- Kept `CONTRIBUTING.md`, `SECURITY.md`, and `CODE_OF_CONDUCT.md` unchanged because this version changes persistence behavior and release documentation, not contributor policy, vulnerability reporting, or community-governance rules.

### For Deletion
- `grade-guard/build-smoke/` (smoke-build output directory still present in the workspace).
- `grade-guard/0.csv` (numbered profile output currently present in the working directory).
- `grade-guard/bolt-4-1-main.exe` (local acceptance-build artifact from the prior Bolt 4.1 validation run).
- `grade-guard/bolt-4-2-main.exe` (local acceptance-build artifact generated while validating Bolt 4.2).
- `grade-guard/tests/vector_test.exe` (local vector regression-test binary present in the workspace).
- `grade-guard/unit-tests/models_lifecycle_test.exe` (local lifecycle regression-test binary present in the workspace).
- `grade-guard/unit-tests/persistence_contract_test.exe` (local persistence-contract regression binary present in the workspace).
- `grade-guard/unit-tests/persistence-blank-lines.csv` (generated blank-line fixture output from the persistence regression).
- `grade-guard/unit-tests/persistence-invalid-number.csv` (generated invalid-number fixture output from the persistence regression).
- `grade-guard/unit-tests/persistence-legacy-fixture.csv` (generated legacy compatibility fixture output from the persistence regression).
- `grade-guard/unit-tests/persistence-missing-field.csv` (generated missing-field fixture output from the persistence regression).
- `grade-guard/unit-tests/persistence-oversized-record.csv` (generated oversized-record fixture output from the persistence regression).
- `grade-guard/unit-tests/persistence-shape-lecture.csv` (generated round-trip lecture-only output from the persistence regression).
- `grade-guard/unit-tests/persistence-shape-lab.csv` (generated round-trip lab-course output from the persistence regression).
- `grade-guard/unit-tests/persistence-shape-mixed.csv` (generated round-trip mixed-profile output from the persistence regression).
- `grade-guard/unit-tests/persistence-runtime/` (generated numbered runtime-output directory from the persistence regression and currently containing `0.csv` and `1.csv`).

## v0.1.9 - 2026-03-13

### Added or Changed
- Updated `README.md` from `v0.1.8` to `v0.1.9`, refreshed the top-level repository status text, redirected the release trail to the new attribution notes, and documented the repository-wide source attribution baseline in the implementation snapshot.
- Added and normalized the required UP Mindanao / BSCS / `CMSC 18 - COMPUTER PROGRAMING I` attribution block, file description section, and in-comment Apache 2.0 license text across every tracked `.h` and `.c` file under `grade-guard/`.
- Added `docs/version-0-1-9-docs.md` with the fuller explanation of the attribution/header normalization scope, file-level provenance goals, validation checks, and manual cleanup guidance for generated artifacts.
- Kept `REQUIREMENTS.md`, `CONTRIBUTING.md`, `SECURITY.md`, and `CODE_OF_CONDUCT.md` unchanged because this version does not alter runtime scope, contributor workflow, security-reporting policy, or community-governance rules.

### For Deletion
- `grade-guard/build-smoke/` (smoke-build output directory still present in the workspace).
- `grade-guard/bolt-4-1-main.exe` (local acceptance-build artifact from the prior Bolt 4.1 validation run).
- `grade-guard/tests/vector_test.exe` (local vector regression-test binary present in the workspace).
- `grade-guard/unit-tests/models_lifecycle_test.exe` (local lifecycle regression-test binary present in the workspace).
- `grade-guard/unit-tests/persistence_contract_test.exe` (local persistence-contract regression binary present in the workspace).
- `grade-guard/unit-tests/persistence-legacy-fixture.csv` (generated legacy compatibility fixture output from the persistence regression).
- `grade-guard/unit-tests/persistence-shape-lecture.csv` (generated round-trip output from the persistence regression).
- `grade-guard/unit-tests/persistence-shape-lab.csv` (generated round-trip output from the persistence regression).
- `grade-guard/unit-tests/persistence-shape-mixed.csv` (generated round-trip output from the persistence regression).
- `grade-guard/unit-tests/persistence-runtime/` (generated numbered runtime-output directory from the persistence regression).

## v0.1.8 - 2026-03-13

### Added or Changed
- Updated `README.md` from `v0.1.7` to `v0.1.8`, refreshed the project status/progress snapshot, corrected stale Unit 3 roadmap items, pointed the release trail at the new persistence-contract notes, and documented the versioned `GRADE_GUARD_CSV,1` persistence model plus its focused regression command.
- Implemented the dedicated persistence contract in `grade-guard/header/persistence.h` and `grade-guard/source/persistence.c`, including a status-based save/load/list API, explicit row-tagged CSV output for new saves, numbered profile-path discovery, and legacy-load compatibility for historical unversioned profile files.
- Updated `grade-guard/source/app.c` so startup and shutdown now consume the persistence boundary for profile discovery, loading, and saving instead of building CSV file names directly, and so persistence failures are surfaced through explicit user-facing error messages.
- Added `grade-guard/unit-tests/persistence_contract_test.c` to validate round-trip persistence across lecture-only, lab, and mixed profile shapes, verify the numbered-file discovery contract, and confirm compatibility with a representative legacy CSV fixture.
- Added `docs/unit-4-bolt-4-1-persistence-contract.md` to document the Bolt 4.1 schema, file-ordering rules, compatibility stance, unsupported text cases, migration behavior, and validation evidence.
- Added `docs/version-0-1-8-docs.md` with the fuller explanation of the persistence-boundary design, migration contract, validation commands, and remaining Unit 4 review gap.
- Updated `CONTRIBUTING.md` because persistence-touching changes now need the dedicated persistence contract regression in addition to the existing targeted checks and any required live console verification.
- Updated `REQUIREMENTS.md` so Unit 4 / Bolt 4.1 is checked off with dated implementation, regression, and review evidence while keeping the broader Unit 4 schema-approval gate open ahead of Bolt 4.2.
- Kept `SECURITY.md` and `CODE_OF_CONDUCT.md` unchanged because this version changes persistence behavior and contributor verification guidance, not vulnerability-reporting policy or community-governance rules.

### For Deletion
- `grade-guard/bolt-4-1-main.exe` (local acceptance-build artifact generated while validating Bolt 4.1).
- `grade-guard/tests/vector_test.exe` (local vector regression-test binary rebuilt during the Bolt 4.1 acceptance run).
- `grade-guard/unit-tests/models_lifecycle_test.exe` (local lifecycle regression-test binary rebuilt during the Bolt 4.1 acceptance run).
- `grade-guard/unit-tests/persistence_contract_test.exe` (local persistence-contract regression binary generated while validating Bolt 4.1).
- `grade-guard/unit-tests/persistence-legacy-fixture.csv` (local runtime fixture generated by the persistence contract regression test).
- `grade-guard/unit-tests/persistence-shape-lecture.csv` (local round-trip output generated by the persistence contract regression test).
- `grade-guard/unit-tests/persistence-shape-lab.csv` (local round-trip output generated by the persistence contract regression test).
- `grade-guard/unit-tests/persistence-shape-mixed.csv` (local round-trip output generated by the persistence contract regression test).
- `grade-guard/unit-tests/persistence-runtime/` (local numbered-profile output directory generated by the persistence contract regression test and currently containing `0.csv` and `1.csv`).

## v0.1.7 - 2026-03-13

### Added or Changed
- Updated `README.md` from `v0.1.6` to `v0.1.7`, refreshed the project status/progress snapshot, documented the extracted `app` and `profile_controller` runtime path, corrected the standard build commands to link every live module, and pointed the release trail at the new Bolt 3.2 notes.
- Implemented the runtime app orchestrator in `grade-guard/source/app.c`, including console startup, numbered-profile bootstrap, top-level menu dispatch, and save/release behavior on exit.
- Implemented the workflow-controller module in `grade-guard/source/profile_controller.c`, including profile selection, profile creation, course operations, parameter operations, activity operations, profile settings, and grade viewing with smaller controller helpers instead of the old monolithic `main.c` flow.
- Updated `grade-guard/main.c` to the intended thin-entry-point shape so the executable now delegates directly to `app_run()`.
- Wired the previously scaffolded runtime support modules in `grade-guard/source/grade_calc.c` and `grade-guard/source/persistence.c` into the live build so the extracted controller path no longer depends on placeholder implementations.
- Added `docs/unit-3-bolt-3-2-workflow-controllers.md` to document the Bolt 3.2 contract, controller/user-flow map, review notes, and non-interactive validation evidence.
- Added `docs/version-0-1-7-docs.md` with the fuller explanation of the controller extraction, runtime module graph, validation commands, and remaining manual-acceptance gap.
- Updated `CONTRIBUTING.md` because the standard build now links the extracted `app`, `profile_controller`, `grade_calc`, and `persistence` modules, and workflow-touching changes now explicitly require a live Windows console verification pass.
- Updated `REQUIREMENTS.md` to record Bolt 3.2 design/implementation/docs progress plus dated compile/regression evidence while keeping the manual workflow test, review, and human-validation gates open.
- Kept `SECURITY.md` and `CODE_OF_CONDUCT.md` unchanged because this version does not alter vulnerability-reporting policy or community-governance rules.

### For Deletion
- `grade-guard/build-smoke/` (compile-smoke object output directory from earlier scaffold validation; still present locally and should not be committed).
- `grade-guard/grade-guard-warning-build.exe` (local warning-check build artifact; still present locally and should not be committed).
- `grade-guard/bolt-2-1-main.exe` (local acceptance-build artifact generated while validating Bolt 2.1).
- `grade-guard/bolt-2-2-main.exe` (local acceptance-build artifact generated while validating Bolt 2.2).
- `grade-guard/bolt-3-1-main.exe` (local acceptance-build artifact generated while validating Bolt 3.1).
- `grade-guard/bolt-3-2-main.exe` (local acceptance-build artifact generated while validating Bolt 3.2).
- `grade-guard/tests/vector_test.exe` (local vector regression-test binary generated while validating Bolts 2.1 through 3.2).
- `grade-guard/unit-tests/models_lifecycle_test.exe` (local lifecycle regression-test binary generated while validating Bolts 2.2 through 3.2).

## v0.1.6 - 2026-03-12

### Added or Changed
- Updated `README.md` from `v0.1.5` to `v0.1.6`, refreshed the project status/progress snapshot, documented the extracted `ui_console` layer, corrected the standard build commands to link `grade-guard/source/ui_console.c`, and pointed the release trail at the new Bolt 3.1 notes.
- Implemented the shared UI/platform module in `grade-guard/header/ui_console.h` and `grade-guard/source/ui_console.c`, including screen setup, cursor and color helpers, selection widgets, field input, failure display, and the new `ui_clear_screen()` / `ui_read_key()` wrappers that isolate direct console calls.
- Updated `grade-guard/main.c` to consume `header/ui_console.h` instead of defining raw Windows console helpers inline, and switched UI call sites to the typed `UI_LIST_*` and `UI_KEY_*` interface exported by the shared console module.
- Corrected a few inherited console-layer defects during extraction, including the `UI_SELECTION_BOTH` fallthrough path, selection-label alphabet wraparound, and a stray variadic initialization misuse inside the menu widget.
- Added `docs/unit-3-bolt-3-1-ui-console.md` to document the Bolt 3.1 contract, Windows-specific dependencies, UI-boundary review notes, and current validation evidence.
- Added `docs/version-0-1-6-docs.md` with the fuller explanation of the UI extraction, release impact, remaining manual-acceptance gap, and updated verification trail.
- Updated `CONTRIBUTING.md` because the standard build now links `ui_console.c`, and UI-touching changes now explicitly require a live Windows console verification pass in addition to compile/test checks.
- Kept `REQUIREMENTS.md`, `SECURITY.md`, and `CODE_OF_CONDUCT.md` unchanged because Bolt 3.1 manual acceptance has not yet been executed and this version does not alter project governance or security-reporting policy.

### For Deletion
- `grade-guard/build-smoke/` (compile-smoke object output directory from earlier scaffold validation; still present locally and should not be committed).
- `grade-guard/grade-guard-warning-build.exe` (local warning-check build artifact; still present locally and should not be committed).
- `grade-guard/bolt-2-1-main.exe` (local acceptance-build artifact generated while validating Bolt 2.1).
- `grade-guard/bolt-2-2-main.exe` (local acceptance-build artifact generated while validating Bolt 2.2).
- `grade-guard/bolt-3-1-main.exe` (local acceptance-build artifact generated while validating Bolt 3.1).
- `grade-guard/tests/vector_test.exe` (local vector regression-test binary generated while validating Bolts 2.1 through 3.1).
- `grade-guard/unit-tests/models_lifecycle_test.exe` (local lifecycle regression-test binary generated while validating Bolts 2.2 and 3.1).

## v0.1.5 - 2026-03-12

### Added or Changed
- Updated `README.md` from `v0.1.4` to `v0.1.5`, refreshed the project status/progress snapshot, and documented the new models-module build/test flow plus the Bolt 2.2 release notes link.
- Implemented the domain lifecycle module in `grade-guard/header/models.h` and `grade-guard/source/models.c`, including `*_init()`, `*_reset()`, and `*_destroy()` helpers for `Activities`, `Course_Parameter`, `Course_Component`, `Course`, and `Student_Profile`.
- Updated `grade-guard/main.c` to consume `header/models.h`, removed duplicate in-file domain declarations/destructors, and aligned create/add/delete flows with vector by-value ownership and `vector_remove()` cleanup semantics.
- Added the first shared lifecycle test harness under `grade-guard/unit-tests/`, including `test_framework.h` and `models_lifecycle_test.c`, then validated it with a local executable acceptance run.
- Added `docs/unit-2-bolt-2-2-models.md` to document the Bolt 2.2 ownership contract, nested invariants, review notes, and acceptance evidence.
- Added `docs/version-0-1-5-docs.md` with the fuller explanation of the Bolt 2.2 extraction, repository-level release trail, validation commands, and remaining review gates.
- Updated `REQUIREMENTS.md` so Unit 2 / Bolt 2.2 is checked off with dated implementation, validation, and review evidence; the Unit 2 human validation line remains open.
- Updated `CONTRIBUTING.md` because contributors now need the `models.c` link step in normal builds and a second focused regression check when touching shared domain ownership flows.
- Updated `SECURITY.md` to correct the supported-version table for the active `0.1.x` line; kept `CODE_OF_CONDUCT.md` unchanged because this version does not alter community-governance policy.

### For Deletion
- `grade-guard/build-smoke/` (compile-smoke object output directory from earlier scaffold validation; still present locally and should not be committed).
- `grade-guard/grade-guard-warning-build.exe` (local warning-check build artifact; still present locally and should not be committed).
- `grade-guard/main.exe` (local compiled binary artifact; still present locally and should not be committed).
- `grade-guard/bolt-2-1-main.exe` (local acceptance-build artifact generated while validating Bolt 2.1).
- `grade-guard/bolt-2-2-main.exe` (local acceptance-build artifact generated while validating Bolt 2.2).
- `grade-guard/tests/vector_test.exe` (local vector regression-test binary generated while validating Bolts 2.1 and 2.2).
- `grade-guard/unit-tests/models_lifecycle_test.exe` (local lifecycle regression-test binary generated while validating Bolt 2.2).

## v0.1.4 - 2026-03-12

### Added or Changed
- Updated `README.md` from `v0.1.3` to `v0.1.4`, refreshed the project status/progress snapshot, and pointed the root documentation at the extracted vector module plus its regression harness.
- Implemented the shared vector module in `grade-guard/header/vector.h` and `grade-guard/source/vector.c`, and removed the duplicate live vector implementation from `grade-guard/main.c`.
- Added `grade-guard/tests/vector_test.c` with repeatable regression coverage for create, push, pop, remove, resize, and indexed access across value and pointer storage cases.
- Added `docs/unit-2-bolt-2-1-vector.md` to document the Bolt 2.1 contract, vector ownership rules, failure behavior, review notes, and acceptance evidence.
- Added `docs/version-0-1-4-docs.md` with the fuller explanation of the Bolt 2.1 vector extraction, validation evidence, build/test workflow updates, and remaining review gates.
- Updated `CONTRIBUTING.md` and `REQUIREMENTS.md` to reflect the new vector-linked build flow, targeted regression check, and Bolt 2.1 completion state.
- Kept `SECURITY.md` and `CODE_OF_CONDUCT.md` unchanged because this version changes implementation structure and contributor verification guidance, not security reporting or community-governance policy.

### For Deletion
- `grade-guard/build-smoke/` (compile-smoke object output directory from earlier scaffold validation; still present locally and should not be committed).
- `grade-guard/grade-guard-warning-build.exe` (local warning-check build artifact; still present locally and should not be committed).
- `grade-guard/main.exe` (local compiled binary artifact; still present locally and should not be committed).
- `grade-guard/bolt-2-1-main.exe` (local acceptance-build artifact generated while validating Bolt 2.1).
- `grade-guard/tests/vector_test.exe` (local regression-test binary generated while validating Bolt 2.1).

## v0.1.3 - 2026-03-12

### Added or Changed
- Added the first module scaffold under `grade-guard/header/` and `grade-guard/source/`, including public headers and compile-safe placeholder source files for `app`, `vector`, `models`, `grade_calc`, `persistence`, `ui_console`, and `profile_controller`.
- Added `docs/unit-1-bolt-1-2-module-scaffold.md` to document the Bolt 1.2 contract, module inventory, include-guard convention, dependency layering rules, shared-constants strategy, ownership rules, and compile-smoke evidence.
- Updated `REQUIREMENTS.md` so Unit 1 / Bolt 1.2 now records completed design, implement, test, and docs subtasks plus dated smoke-build evidence; review and human validation remain open.
- Updated `README.md` from `v0.1.2` to `v0.1.3` and refreshed the project status, progress checkpoint, and roadmap to reflect that the interface scaffold now exists and has been compile-checked.
- Added `docs/version-0-1-3-docs.md` with the fuller explanation of the Bolt 1.2 scaffold, why the source tree remains placeholder-only, and what still requires human approval before extraction accelerates.
- Kept `CONTRIBUTING.md`, `SECURITY.md`, and `CODE_OF_CONDUCT.md` unchanged because this version changes repository structure and release documentation, not contributor workflow, governance policy, or vulnerability reporting flow.

### For Deletion
- `grade-guard/build-smoke/` (compile-smoke object output directory generated while validating the new scaffold; should not be committed).
- `grade-guard/grade-guard-warning-build.exe` (local warning-check binary artifact; should not be committed).
- `grade-guard/main.exe` (local compiled binary artifact; should not be committed).

## v0.1.2 - 2026-03-12

### Added or Changed
- Added `docs/unit-1-bolt-1-1-monolith-inventory.md` to inventory `grade-guard/main.c`, define the module ownership charter, map top-level functions to destination modules, and document the thin-entry-point target for the future refactor.
- Updated the local planning baseline in `REQUIREMENTS.md` so Unit 1 / Bolt 1.1 now records completed design, implementation, test, and docs subtasks plus evidence from the static coverage check; human review remains open.
- Updated `README.md` from `v0.1.1` to `v0.1.2` and pointed the project overview at the new architecture inventory and Bolt 1.1 progress state.
- Added `docs/version-0-1-2-docs.md` with the fuller explanation of the Unit 1 / Bolt 1.1 inventory, validation method, remaining review gate, and manual cleanup notes.
- Kept `CONTRIBUTING.md`, `SECURITY.md`, and `CODE_OF_CONDUCT.md` unchanged because this version extends architecture-planning documentation rather than contributor policy, governance, or security reporting flow.

### For Deletion
- `grade-guard/grade-guard-warning-build.exe` (local warning-check binary generated during compile validation; should not be committed).
- `grade-guard/main.exe` (local compiled binary artifact; should not be committed).

## v0.1.1 - 2026-03-12

### Added or Changed
- Added root `REQUIREMENTS.md` as the planning baseline for the next engineering phase, including intent, assumptions, user stories, NFRs, risk register, Units, Bolts, architecture, testing, and operations coverage for the Grade Guard refactor.
- Updated `README.md` from `v0.1.0` to `v0.1.1` and documented that the repository now includes a requirements-driven plan for modularizing `grade-guard/main.c` into `header/` and `source/` directories.
- Added `docs/version-0-1-1-docs.md` with a fuller explanation of the new requirements artifact, the intended refactor path, and the validation notes that informed the planning scope.
- Kept `CONTRIBUTING.md`, `SECURITY.md`, and `CODE_OF_CONDUCT.md` unchanged because this version adds planning and release documentation rather than changing contributor policy or security reporting expectations.

### For Deletion
- `grade-guard/grade-guard-warning-build.exe` (local warning-check binary generated during compile validation; should not be committed).
- `grade-guard/main.exe` (local compiled binary artifact; should not be committed).
- `grade-guard/tempCodeRunnerFile.c` (editor-generated scratch file; should not be committed).

## v0.1.0 - 2026-03-12

### Added or Changed
- Added the initial Grade Guard prototype source tree under `grade-guard/`, anchored by the Windows console implementation in `grade-guard/main.c`.
- Updated `README.md` from `v0.0.1` to `v0.1.0` so the repository now documents the concrete profile, course, activity, persistence, and grade-goal workflows present in code.
- Added `docs/version-0-1-0-docs.md` with a fuller walkthrough of the imported prototype, current limitations, and cleanup notes for local artifacts.
- Kept `CONTRIBUTING.md`, `SECURITY.md`, and `CODE_OF_CONDUCT.md` unchanged because the prototype source import does not invalidate the governance baseline introduced in `v0.0.1`.

### For Deletion
- `grade-guard/main.exe` (local compiled binary artifact; should not be committed).
- `grade-guard/tempCodeRunnerFile.c` (editor-generated scratch file; should not be committed).

## v0.0.1 - 2026-03-12

### Added or Changed
- Initialized root repository documentation for Grade Guard.
- Replaced generic or template root docs with project-specific `README.md`, `CODE_OF_CONDUCT.md`, `CONTRIBUTING.md`, and `SECURITY.md`.
- Documented the current Apache 2.0 licensing, maintainer contact details, and project metadata from `agent/init.cfg`.
- Added `docs/version-0-0-1-docs.md` to capture detailed version notes for the initial repository baseline.
- Updated `.gitignore` to ignore `.gitignore` itself and hide the agent workflow files from future commits, as required by the initialization task.

### For Deletion
- None from this task context (documentation-only initialization; no generated build artifacts were created).

