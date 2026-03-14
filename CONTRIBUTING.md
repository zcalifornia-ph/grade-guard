# Contributing

Thanks for contributing to Grade Guard.
This repository currently centers on a Windows console C application with a thin `grade-guard/main.c` entry point plus extracted modules under `grade-guard/header/` and `grade-guard/source/`, and the root documentation that explains how the project is used and maintained.

## Before You Start

- Review open issues and pull requests before starting work.
- Open an issue first if you plan to change major behavior, file formats, or the user workflow.
- Keep contributions narrow in scope and easy to review.
- Read `CODE_OF_CONDUCT.md`, `SECURITY.md`, and `LICENSE.txt` before submitting changes.

## Development Setup

Validated baseline setup:

```powershell
git clone https://github.com/zcalifornia-ph/grade-guard.git
cd grade-guard
powershell -ExecutionPolicy Bypass -File .\scripts\build.ps1
.\artifacts\unit-6-bolt-6-1\grade-guard.exe
```

This baseline assumes PowerShell plus a GCC-compatible Windows C toolchain on `PATH`.
Use the manual compile commands in `README.md` only if you are deliberately debugging the raw compile line instead of the validated script path.

## Contribution Standards

- Preserve or improve clarity in the grade-tracking workflow.
- Keep C code readable and close to the existing project style unless a larger refactor is justified.
- Document user-visible changes in `README.md` and `CHANGELOG.md` when relevant.
- Include manual verification steps plus any targeted automated checks relevant to your change.
- Avoid committing generated CSV profile files or ignored agent-workflow files.
- Do not introduce platform-specific behavior beyond Windows unless the goal of the change is explicit portability work.

## Recommended Verification

Before opening a pull request, verify the relevant flows:

1. Run `powershell -ExecutionPolicy Bypass -File .\scripts\build.ps1` to confirm the validated application build still works.
2. Run `powershell -ExecutionPolicy Bypass -File .\scripts\validate.ps1` if your change touches shared runtime modules, release/readme documentation, validation scripts, or more than one regression area.
3. Run the focused vector regression test if your change touches the shared vector module and you are not already using the full validation script.
4. Run the lifecycle regression test if your change touches `models`, nested ownership, or create/delete flows and you are not already using the full validation script.
5. Run the persistence contract regression test if your change touches `persistence`, CSV schema rules, numbered-profile discovery, or startup/shutdown save/load behavior and you are not already using the full validation script.
6. Run a live Windows console verification pass if your change touches `ui_console`, `app`, `profile_controller`, menu navigation, field editing, screen redraw logic, or other keyboard-driven UI behavior.
7. Create a new profile.
8. Add at least one course and define its parameters.
9. Record at least one activity score.
10. View the resulting grade breakdown.
11. Exit and confirm the profile data persists to numbered CSV files in the working directory.

## Pull Requests

Open a pull request with:

- a short problem statement
- the approach you took
- manual verification notes
- screenshots or terminal output when the user interface or output format changed
- risks or follow-up work, if any

## Branch and Commit Guidance

Short-lived branches are preferred.
Conventional Commit prefixes are encouraged, for example:

```text
feat: add CSV import validation
fix: prevent invalid score entry from corrupting output
docs: update Windows build instructions
```

## Security Reporting

Do not report vulnerabilities in public issues.
Use the process described in `SECURITY.md`.

## Community Conduct

All participation in this repository is governed by `CODE_OF_CONDUCT.md`.
