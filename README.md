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
    <strong>Grade Guard is a Windows console application for tracking semester courses, assessment weights, recorded scores, and projected academic standing.</strong>
    <br />
    Version: v0.1.3
    <br />
    Status: first source-backed prototype / Windows-only academic codebase with Unit 1 interface scaffold and smoke-verified module layout
    <br />
    <a href="https://github.com/zcalifornia-ph/grade-guard"><strong>Explore the repository</strong></a>
    <br />
    <a href="docs/version-0-1-3-docs.md"><strong>Version 0.1.3 notes</strong></a>
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
        <li><a href="#current-planning-baseline">Current Planning Baseline</a></li>
        <li><a href="#current-implementation-snapshot">Current Implementation Snapshot</a></li>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#build-and-run">Build and Run</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>

## About The Project

Grade Guard began as a CMSC 18 final project at the University of the Philippines Mindanao.
It is designed to help students monitor their academic progress on a per-semester basis by modeling courses, assessment categories, activity scores, and goal grades inside a text-based interface.

The current implementation now ships in-repository under `grade-guard/main.c` and uses a weighted-average approach to estimate student standing from the scores and course weights entered by the user.
The repository also now includes a root `REQUIREMENTS.md` that defines the next engineering phase: splitting the monolithic source into `header/` and `source/` modules while fixing verified defects.
That planning baseline is now accompanied by `docs/unit-1-bolt-1-1-monolith-inventory.md`, which maps the current monolith responsibilities and proposed destination modules before extraction starts, and `docs/unit-1-bolt-1-2-module-scaffold.md`, which records the initial public interface scaffold for that refactor.
Detailed version notes for this scaffold update are available in `docs/version-0-1-3-docs.md`.

### What Grade Guard Does

- Creates student profiles with basic academic identity data.
- Tracks multiple courses, including units and optional laboratory components.
- Lets users define course parameters such as quizzes, exams, and assignments with custom weights.
- Records activity scores under each parameter and displays grade details in the console interface.
- Stores profile data in numbered CSV files such as `0.csv`, `1.csv`, and so on in the working directory.
- Supports grade-goal selection, including custom targets.
- Calculates a predicted GWA and the percentage progress toward the selected grade goal.

### Current Limitations

- Windows-only for now because the program depends on `windows.h` and `conio.h`.
- Implemented as a single large C source file, so modularity and testability are still limited.
- Uses manual CSV persistence with no automated migration, validation suite, or release pipeline yet.
- Local binaries and editor scratch files still require manual cleanup before committing.

### Current Planning Baseline

- Refactor target: split `grade-guard/main.c` into focused modules under `grade-guard/header/` and `grade-guard/source/`.
- Entry-point target: keep `grade-guard/main.c` small and orchestration-only.
- Defect focus: memory ownership, string/buffer safety, CSV parsing robustness, selection flow, and grade-calculation edge cases.
- Planning artifact: `REQUIREMENTS.md`
- Architecture inventory: `docs/unit-1-bolt-1-1-monolith-inventory.md`
- Scaffold inventory: `docs/unit-1-bolt-1-2-module-scaffold.md`
- Progress checkpoint: Bolt 1.1 responsibility mapping and Bolt 1.2 scaffold creation plus compile-smoke evidence are recorded; Unit 1 human review is still pending before those Bolts can be closed.

### Current Implementation Snapshot

- Entry point: `grade-guard/main.c`
- Planned extraction scaffold: `grade-guard/header/` and `grade-guard/source/`
- UI model: keyboard-driven Windows console interface using arrow keys, `Enter`, and screen clearing / cursor positioning helpers
- Core data model: dynamic vectors for `Student_Profile`, `Course`, `Course_Parameter`, and `Activities`
- Academic model: lecture components plus optional laboratory components, each with weighted parameters and activity scores
- Persistence model: numbered CSV files written in the working directory when the user exits from the main menu

### Built With

- [C](https://en.wikipedia.org/wiki/C_(programming_language))
- [Windows Console API](https://learn.microsoft.com/windows/console/)
- Standard C library facilities for file I/O, memory management, and string handling

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Getting Started

### Prerequisites

- Windows 10 or Windows 11
- A C compiler that can build code using `windows.h` and `conio.h`
- Git, if you want to clone the repository directly

### Build and Run

Clone the repository:

```sh
git clone https://github.com/zcalifornia-ph/grade-guard.git
cd grade-guard
```

Build with GCC or MinGW-w64:

```sh
gcc grade-guard/main.c -o grade-guard.exe
```

Build with MSVC Developer PowerShell:

```powershell
cl /TC grade-guard\main.c /Fe:grade-guard.exe
```

Run the application:

```powershell
.\grade-guard.exe
```

When you exit through the main menu, the program writes profile data back to numbered CSV files in the current working directory.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Usage

Typical flow inside the application:

Navigation is keyboard-driven through the Windows console UI and relies on arrow keys plus `Enter`.

1. Select `New Profile` and enter your student details.
2. Add one or more courses, including units and whether a course has a laboratory component.
3. Define weighted course parameters such as quizzes, exams, projects, or assignments.
4. Add activities and record achieved scores for each parameter.
5. Set a target grade goal from the preset options or enter a custom goal.
6. Use `View Grades` to inspect the current breakdown.
7. Exit the program to persist loaded profiles back to numbered CSV files in the working directory.

You can also reopen an existing profile by student number through the `Select Profile` flow in the main menu.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Roadmap

- [ ] Finalize Unit 1 / Bolt 1.1 review and confirm the proposed module boundaries.
- [ ] Review the new `grade-guard/header/` and `grade-guard/source/` scaffold and confirm the public interfaces are stable enough for extraction.
- [ ] Begin moving `vector`, `models`, and other owned implementation out of `grade-guard/main.c` into the new scaffold without breaking current behavior.
- [ ] Improve validation for score entry, CSV parsing, and edge cases.
- [ ] Add a repeatable build workflow and automated verification.
- [ ] Expand grade summaries and reporting for easier semester planning.
- [ ] Evaluate cross-platform terminal support after the Windows prototype stabilizes.

See the [open issues](https://github.com/zcalifornia-ph/grade-guard/issues) for planned improvements and known gaps.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Contributing

Contributions are welcome.
Read `CONTRIBUTING.md`, `CODE_OF_CONDUCT.md`, and `SECURITY.md` before opening a pull request.

1. Fork the project.
2. Create a branch for your change.
3. Implement and manually verify the update.
4. Document user-visible changes where needed.
5. Open a pull request with a clear summary and verification notes.

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

- Ravhen M. Grageda, Charisse C. Lorejo, and Jhaye Marie H. Gonzales for the original project team named in the source header.
- University of the Philippines Mindanao, where the project originated as a course deliverable.
- Students and contributors interested in practical academic tracking tools.

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
