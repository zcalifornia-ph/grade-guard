# Contributing

Thanks for contributing to Grade Guard.
This repository currently centers on a Windows console C application in `grade-guard/main.c`, plus the root documentation that explains how the project is used and maintained.

## Before You Start

- Review open issues and pull requests before starting work.
- Open an issue first if you plan to change major behavior, file formats, or the user workflow.
- Keep contributions narrow in scope and easy to review.
- Read `CODE_OF_CONDUCT.md`, `SECURITY.md`, and `LICENSE.txt` before submitting changes.

## Development Setup

Example setup with GCC or MinGW-w64:

```sh
git clone https://github.com/zcalifornia-ph/grade-guard.git
cd grade-guard
gcc grade-guard/main.c -o grade-guard.exe
.\grade-guard.exe
```

Example setup with MSVC Developer PowerShell:

```powershell
git clone https://github.com/zcalifornia-ph/grade-guard.git
cd grade-guard
cl /TC grade-guard\main.c /Fe:grade-guard.exe
.\grade-guard.exe
```

## Contribution Standards

- Preserve or improve clarity in the grade-tracking workflow.
- Keep C code readable and close to the existing project style unless a larger refactor is justified.
- Document user-visible changes in `README.md` and `CHANGELOG.md` when relevant.
- Include manual verification steps because the repository does not currently ship with an automated test suite.
- Avoid committing generated CSV profile files or ignored agent-workflow files.
- Do not introduce platform-specific behavior beyond Windows unless the goal of the change is explicit portability work.

## Recommended Verification

Before opening a pull request, manually verify the main user flows:

1. Build the executable successfully.
2. Create a new profile.
3. Add at least one course and define its parameters.
4. Record at least one activity score.
5. View the resulting grade breakdown.
6. Exit and confirm the profile data persists to numbered CSV files in the working directory.

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
