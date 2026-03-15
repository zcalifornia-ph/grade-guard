# Version 1.0.1 Notes

## Title
Grade Guard README Demo Walkthrough Documentation Patch

## Quick Diagnostic Read

You are looking at the correct `v1.0.1` repository state if it now contains:

- a root `README.md` updated to `v1.0.1`,
- a new top `v1.0.1` entry in `CHANGELOG.md`,
- this file at `docs/version-1-0-1-docs.md`,
- a populated `repo/demo/` directory with the ordered CLI screenshots,
- and a `README.md` `Usage` section that now includes `Controls`, `CLI Walkthrough`, and `Resume a Saved Profile`.

The governance and contributor-workflow documents changed only where this pass required it:

- `CONTRIBUTING.md` was not changed because the contributor workflow did not change in this patch.
- `SECURITY.md` was not changed because vulnerability-reporting expectations did not change in this patch.
- `CODE_OF_CONDUCT.md` was not changed because this release does not alter community-governance rules.

## One-Sentence Objective

Publish a documentation-only patch that turns the Grade Guard README into a visual, step-by-step CLI how-to backed by real demo screenshots.

## Why This Version Matters

`v1.0.0` established the stable release-facing repository baseline, but the main usage guidance was still mostly textual.
That was enough for someone already familiar with the application, but weaker for a first-time reviewer who wanted to understand the CLI flow quickly.

`v1.0.1` closes that documentation gap.
It does not add new runtime behavior, change the validation scripts, or alter the grading engine.
Instead, it makes the shipped workflow more legible:

- the usage section now shows the real screen order,
- the keyboard interaction model is documented explicitly,
- the README now explains how a saved profile is reopened,
- and the repository now carries the ordered demo images that the walkthrough depends on.

This matters because the application is console-driven.
For this project, seeing the menu order and prompt wording is a large part of understanding how the tool is actually used.

## What Changed (Artifact Map)

- Root release metadata:
  - `README.md`
  - `CHANGELOG.md`
- Version-specific release note:
  - `docs/version-1-0-1-docs.md`
- Demo assets referenced by the README:
  - `repo/demo/grade-guard- (1).png` through `repo/demo/grade-guard- (24).png`

## Guided Walkthrough

## 1) The README Usage Section Is Now a Real Walkthrough

Before this patch, `README.md` described the Grade Guard workflow in a short numbered list.
That gave the major actions, but it did not show:

- which menu names users actually see,
- where the goal-selection step appears,
- how course parameters and activities are entered,
- or what the final grade review screen looks like.

The new walkthrough now reconstructs the CLI session in order, using the actual screenshots and the actual prompt names.

## 2) The Demo Images Are Ordered Around the Real CLI Flow

The new `repo/demo/` sequence now supports the README as a walkthrough asset set rather than a loose screenshot dump.
The images now document:

- profile creation,
- first-course entry,
- grade-goal selection,
- profile reloading by student number,
- course-parameter setup,
- activity creation,
- score entry,
- and final grade inspection.

That ordering matters because Grade Guard is stateful.
The meaning of later screens depends on the earlier setup choices, so the README needed the screenshots in the same order that the CLI expects them.

## 3) The Walkthrough Was Matched Against the Actual Source Prompts

This patch did not just paste screenshots into the README.
The screen order and explanations were matched against the current controller/menu names in the application source so the documentation aligns with the shipped CLI.

That matters because console apps become confusing quickly if the docs describe menu names or sequences that no longer exist.

## 4) The Repository Layout Note Now Mentions `repo/demo/`

The project-layout section now explicitly mentions that `repo/` holds repository-facing screenshots and demo images.
Without that note, the new README references would work, but the documented repository structure would lag behind the files the README depends on.

## Validation / Review Notes

This patch is documentation-only.
No application source files, tests, or PowerShell scripts were changed.

Because of that, the full validation workflow was not rerun for `v1.0.1`.
Instead, this pass was checked by:

- reading the demo screenshots in `repo/demo/`,
- matching the screenshot sequence to the current CLI menus and prompts in the source tree,
- confirming the new README image references point at real files,
- and confirming the README table of contents and subsection hierarchy remain coherent after the walkthrough expansion.

## Cleanup Notes

Unlike the generated files listed in `CHANGELOG.md`, the new `repo/demo/` images are intentional repository assets for this patch and should be staged with the documentation changes rather than deleted.

The manual cleanup list still applies to the generated binaries, CSV outputs, and runtime directories that already exist in the workspace.
Use the top `v1.0.1` `CHANGELOG.md` entry as the current cleanup reference.

## Immediate Next Actions

1. Stage `README.md`, `CHANGELOG.md`, `docs/version-1-0-1-docs.md`, and the full `repo/demo/` directory together.
2. Preview the README rendering in your Git client or on GitHub before publishing so the embedded screenshot paths are confirmed visually.
3. Keep `CONTRIBUTING.md`, `SECURITY.md`, and `CODE_OF_CONDUCT.md` unchanged unless a later task actually changes contributor process, security policy, or governance expectations.
