<a id="readme-top"></a>

[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![Apache 2.0][license-shield]][license-url]
[![LinkedIn][linkedin-shield]][linkedin-url]

[![Grade Guard Screen Shot][product-screenshot]][repo-url]

<div align="center">
  <h3 align="center">Grade Guard</h3>

  <p align="center">
    <strong>Grade Guard is a Windows console application for planning, tracking, and reviewing semester grades from local student profiles.</strong>
    <br />
    Version: v1.0.0
    <br />
    Status: 1.0.0 release baseline for the Windows console application, with automated validation for core data, persistence, grade calculation, and app orchestration flows.
    <br />
    <a href="https://github.com/zcalifornia-ph/grade-guard"><strong>Explore the repository</strong></a>
    <br />
    <a href="docs/version-1-0-0-docs.md"><strong>Version 1.0.0 notes</strong></a>
    <br />
    <br />
    <a href="https://github.com/zcalifornia-ph/grade-guard/issues">Report Bug</a>
    &middot;
    <a href="https://github.com/zcalifornia-ph/grade-guard/issues">Request Feature</a>
  </p>
</div>

<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#what-grade-guard-does">What Grade Guard Does</a></li>
        <li><a href="#current-limitations">Current Limitations</a></li>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li><a href="#project-layout">Project Layout</a></li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#build-and-run">Build and Run</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#validation">Validation</a></li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>

## About The Project

Grade Guard began as a CMSC 18 final project at the University of the Philippines Mindanao.
It helps students keep a local academic record for one semester at a time, with support for course setup, weighted assessment categories, recorded scores, and projected academic standing.

The current codebase ships as a Windows-first C application with a thin `grade-guard/main.c` entry point and focused runtime modules for application flow, profile management, persistence, console UI helpers, grade calculation, shared data models, and dynamic vectors.

### What Grade Guard Does

- Creates local student profiles with academic identity details and a target grade goal.
- Tracks courses with lecture-only or lecture-plus-laboratory setups.
- Lets users define weighted assessment categories such as quizzes, exams, projects, and assignments.
- Records activity scores and shows the resulting breakdown in a keyboard-driven console interface.
- Calculates predicted GWA and progress toward the selected goal.
- Saves new profiles with the versioned `GRADE_GUARD_CSV,1` format and can still load older unversioned profile files.

### Current Limitations

- Windows-only for now because the runtime depends on `windows.h` and `conio.h`.
- Console redraw, fullscreen, and raw-key behavior still depend on the active Windows console host and benefit from live manual verification.
- Numeric entry is bounded safely inside the grade engine, but malformed text is not yet rejected at every controller input point.
- Persistence remains local-file based; there is no sync, export service, or multi-user storage layer.

### Built With

- [C](https://en.wikipedia.org/wiki/C_(programming_language))
- [Windows Console API](https://learn.microsoft.com/windows/console/)
- [PowerShell](https://learn.microsoft.com/powershell/)
- GCC-compatible Windows toolchains such as MinGW-w64 or TDM-GCC

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Project Layout

```text
grade-guard/
  grade-guard/
    main.c
    header/
      app.h
      grade_calc.h
      models.h
      persistence.h
      profile_controller.h
      ui_console.h
      vector.h
    source/
      app.c
      grade_calc.c
      models.c
      persistence.c
      profile_controller.c
      ui_console.c
      vector.c
    tests/
      vector_test.c
    unit-tests/
      app_smoke_test.c
      models_lifecycle_test.c
      persistence_contract_test.c
      test_framework.h
      unit5_defect_baseline_test.c
  docs/
    version-1-0-0-docs.md
    ...
  scripts/
    build.ps1
    validate.ps1
```

The `docs/` directory keeps detailed release notes and implementation notes.
The root documentation focuses on how to build, validate, and use the application.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Getting Started

### Prerequisites

- Windows 10 or Windows 11
- PowerShell
- A GCC-compatible Windows C toolchain on `PATH`
- Git, if you want to clone the repository directly

### Build and Run

Clone the repository:

```sh
git clone https://github.com/zcalifornia-ph/grade-guard.git
cd grade-guard
```

Build the application with the default release-facing script:

```powershell
powershell -ExecutionPolicy Bypass -File .\scripts\build.ps1
```

Run the application:

```powershell
.\artifacts\build\grade-guard.exe
```

If you want a different output path, pass `-OutputPath` explicitly:

```powershell
powershell -ExecutionPolicy Bypass -File .\scripts\build.ps1 -OutputPath .\artifacts\custom\grade-guard.exe
```

When you exit from the main menu, the program saves profile data back to numbered CSV files in the working directory.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Usage

Navigation is keyboard-driven through the Windows console UI and relies on arrow keys, `Enter`, and `Esc` on supported selection screens.

Typical flow inside the application:

1. Select `New Profile` and enter your student details.
2. Add one or more courses and decide whether each course has a laboratory component.
3. Define weighted assessment categories such as quizzes, exams, or assignments.
4. Add activities and record achieved scores for each category.
5. Select a target grade goal from the presets or enter a custom goal.
6. Open `View Grades` to inspect the current breakdown and predicted GWA.
7. Exit the program to persist the profile locally.

You can reopen an existing profile later by student number through the `Select Profile` flow in the main menu.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Validation

Run the full validation workflow:

```powershell
powershell -ExecutionPolicy Bypass -File .\scripts\validate.ps1
```

That workflow rebuilds the application into `artifacts/validation/` and executes:

- `grade-guard/tests/vector_test.c`
- `grade-guard/unit-tests/models_lifecycle_test.c`
- `grade-guard/unit-tests/persistence_contract_test.c`
- `grade-guard/unit-tests/unit5_defect_baseline_test.c`
- `grade-guard/unit-tests/app_smoke_test.c`

The validation path covers vector behavior, nested model lifecycle rules, CSV persistence, grade-engine edge cases, and application startup/save/load/delete orchestration through a smoke harness.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Roadmap

- Tighten controller-side numeric input validation so malformed text is rejected earlier.
- Expand semester summary and reporting views.
- Add broader automated coverage around interactive console workflows.
- Evaluate a portable terminal abstraction after the Windows-first release baseline stabilizes.

See the [open issues](https://github.com/zcalifornia-ph/grade-guard/issues) for planned improvements and known gaps.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Contributing

Contributions are welcome.
Read `CONTRIBUTING.md`, `CODE_OF_CONDUCT.md`, and `SECURITY.md` before opening a pull request.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## License

Distributed under the Apache 2.0 License.
See `LICENSE.txt` for the full license text.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Contact

Maintainer: Zildjian E. California

Email: `zecalifornia@up.edu.ph`

Project Link: [https://github.com/zcalifornia-ph/grade-guard](https://github.com/zcalifornia-ph/grade-guard)

LinkedIn: [linkedin.com/in/zcalifornia](https://linkedin.com/in/zcalifornia)

ORCID: [0009-0002-2357-7606](https://orcid.org/0009-0002-2357-7606)

ResearchGate: [Zildjian-California](https://www.researchgate.net/profile/Zildjian-California)

Twitter: [@zcalifornia_](https://twitter.com/zcalifornia_)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Acknowledgments

- Ravhen M. Grageda, Charisse C. Lorejo, and Jhaye Marie H. Gonzales for the original project team named in the source headers.
- University of the Philippines Mindanao, where the project originated as a course deliverable.
- Students and contributors interested in practical academic planning tools.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

[contributors-shield]: https://img.shields.io/github/contributors/zcalifornia-ph/grade-guard.svg?style=for-the-badge
[contributors-url]: https://github.com/zcalifornia-ph/grade-guard/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/zcalifornia-ph/grade-guard.svg?style=for-the-badge
[forks-url]: https://github.com/zcalifornia-ph/grade-guard/network/members
[stars-shield]: https://img.shields.io/github/stars/zcalifornia-ph/grade-guard.svg?style=for-the-badge
[stars-url]: https://github.com/zcalifornia-ph/grade-guard/stargazers
[issues-shield]: https://img.shields.io/github/issues/zcalifornia-ph/grade-guard.svg?style=for-the-badge
[issues-url]: https://github.com/zcalifornia-ph/grade-guard/issues
[license-shield]: https://img.shields.io/github/license/zcalifornia-ph/grade-guard.svg?style=for-the-badge
[license-url]: https://github.com/zcalifornia-ph/grade-guard/blob/main/LICENSE.txt
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/zcalifornia
[product-screenshot]: repo/images/project_screen.png
[repo-url]: https://github.com/zcalifornia-ph/grade-guard
