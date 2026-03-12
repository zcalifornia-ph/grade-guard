# Version 0.0.1 Notes

## Title
Initial Repository Documentation and Governance Baseline for Grade Guard

## Quick Diagnostic Read

This version formalizes the repository around the existing Windows console prototype in `grade-guard/main.c`.
You are looking at the correct baseline if the repository now has:

- a project-specific root `README.md`,
- a populated root `CHANGELOG.md`,
- repository governance docs (`CODE_OF_CONDUCT.md`, `CONTRIBUTING.md`, `SECURITY.md`),
- an Apache 2.0 `LICENSE.txt`,
- ignore rules that hide the local agent workflow files from future commits.

## One-Sentence Objective

Make the Grade Guard repository understandable, buildable, and maintainable for first-time readers and future contributors without changing the core C implementation.

## Why This Version Matters

Before this version, the root documentation state was incomplete or generic:

- `README.md` was still a blank-template scaffold,
- `CHANGELOG.md` was empty,
- governance files still carried template language and placeholder contacts,
- `.gitignore` had initialization comments instead of active ignore rules.

That made the repository harder to publish, review, or hand off.
Version `0.0.1` establishes the first coherent public-facing baseline.

## Plan A / Plan B

### Plan A (Recommended): Understand the Project in 15-20 Minutes

1. Read the root `README.md` for project scope, build steps, and usage flow.
2. Skim `CHANGELOG.md` for the release summary.
3. Review `CONTRIBUTING.md`, `CODE_OF_CONDUCT.md`, and `SECURITY.md` for collaboration rules.
4. Open `grade-guard/main.c` to inspect the actual prototype implementation the docs describe.

### Plan B: Verify the Documentation Against the Code

1. Start from `grade-guard/main.c`.
2. Confirm the main menu, profile flow, course flow, and CSV persistence behavior.
3. Compare those behaviors with the README sections on usage, limitations, and setup.

Use Plan B if you want to validate that the docs match the current implementation rather than reading them as standalone repository materials.

## What Changed (Artifact Map)

- Root product overview and setup guide:
  - `README.md`
- Release summary:
  - `CHANGELOG.md`
- Community and contribution rules:
  - `CODE_OF_CONDUCT.md`
  - `CONTRIBUTING.md`
  - `SECURITY.md`
- Ignore rules for local repository workflow files:
  - `.gitignore`
- Existing license retained as the canonical project license:
  - `LICENSE.txt`
- Source of truth for implementation behavior referenced by the docs:
  - `grade-guard/main.c`
- Screenshot asset referenced by the README:
  - `repo/images/project_screen.png`

## Guided Walkthrough (What Was Updated)

## 1) README Replaced the Generic Template

The old README was still placeholder content.
It did not describe Grade Guard, the actual repository, or the build process.

The new README now:

- identifies Grade Guard as a Windows console grade-tracking application,
- names the repository version as `v0.0.1`,
- links to the real GitHub slug,
- references the existing screenshot asset in `repo/images/`,
- documents the current build flow using GCC/MinGW-w64 or MSVC,
- explains the current usage model from the codebase,
- records present limitations such as Windows-only dependencies and single-file structure.

## 2) Changelog Was Initialized

The root changelog now has a real `v0.0.1` entry instead of an empty file.
It records the documentation baseline, governance-document setup, license metadata alignment, and ignore-rule activation.

It also explicitly marks that no generated artifacts were produced by this version, so there are no cleanup actions required from this task.

## 3) Governance Files Were Made Repository-Specific

`CODE_OF_CONDUCT.md`, `CONTRIBUTING.md`, and `SECURITY.md` already existed, but they still contained template or placeholder material.
They were updated because leaving placeholder contacts and generic wording would make the repository misleading.

The updates focused on:

- replacing placeholder reporting addresses with `zecalifornia@up.edu.ph`,
- aligning contribution guidance with a Windows C codebase,
- defining expected manual verification flow,
- narrowing the security policy to the risks that fit this project,
- keeping the documents practical instead of boilerplate.

## 4) .gitignore Was Activated

The previous `.gitignore` only contained comments telling an initializer to uncomment lines later.
This version converts those lines into active ignore rules for:

- `.gitignore`,
- `AGENTS.md`,
- `agent/`,
- `REQUIREMENTS.md`.

That keeps the local agent workflow and self-ignore behavior out of future commits, matching the repository instructions for initialization.

## 5) License Was Intentionally Left As-Is

`LICENSE.txt` already contained a valid Apache 2.0 license text.
It was not rewritten because the initialization scope was to replace placeholders where needed, and the license file was already implemented.

This is an intentional non-change and part of the release rationale.

## Copy-Paste Commands

### Inspect the main implementation file

```powershell
Get-Content -Path grade-guard/main.c
```

### Build with GCC or MinGW-w64

```powershell
gcc grade-guard/main.c -o grade-guard.exe
.\grade-guard.exe
```

### Build with MSVC Developer PowerShell

```powershell
cl /TC grade-guard\main.c /Fe:grade-guard.exe
.\grade-guard.exe
```

## Pitfalls + Review Notes

### 1) Do not treat this as a feature release beyond the current prototype

This version documents the current codebase.
It does not claim that the implementation was refactored, tested, or redesigned.

### 2) The README is accurate to the current Windows-specific implementation

That means the documented platform limits are deliberate.
If the code later becomes portable, the README should be updated again rather than assuming that portability now exists.

### 3) The docs are based on actual repository state, not aspirational structure

Examples:

- the implementation is still centered in one C file,
- CSV persistence still happens through numbered files in the working directory,
- the screenshot path points to the real `repo/images/project_screen.png` asset.

## Skill Transfer: What This Version Clarifies

After reading this version’s docs, a contributor should be able to explain:

1. What Grade Guard is supposed to do.
2. How the current prototype is built and run.
3. Which repository files govern collaboration and security reporting.
4. Which parts of the repository were intentionally documented versus intentionally left unchanged.

## 24-72 Hour Next Steps

1. Compile the current prototype and confirm the documented build instructions work in your preferred Windows toolchain.
2. Review `grade-guard/main.c` for opportunities to separate data structures, UI handling, and persistence into smaller modules.
3. Decide whether future versions will stay Windows-only or begin a portability pass.
4. If code changes land next, update the changelog and add a new version-notes document for that release rather than overwriting this one.

---

This file exists to explain `v0.0.1` in more detail than the changelog while staying grounded in the repository’s actual state.
