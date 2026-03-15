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
    Version: v1.0.1
    <br />
    Status: 1.0.1 documentation patch on top of the 1.0.0 release baseline, adding a screenshot-backed CLI walkthrough and demo assets for the Windows console application.
    <br />
    <a href="https://github.com/zcalifornia-ph/grade-guard"><strong>Explore the repository</strong></a>
    <br />
    <a href="docs/version-1-0-1-docs.md"><strong>Version 1.0.1 notes</strong></a>
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
    <li>
      <a href="#usage">Usage</a>
      <ul>
        <li><a href="#controls">Controls</a></li>
        <li><a href="#cli-walkthrough">CLI Walkthrough</a></li>
        <li><a href="#resume-a-saved-profile">Resume a Saved Profile</a></li>
      </ul>
    </li>
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
    version-1-0-1-docs.md
    ...
  repo/
    demo/
    images/
  scripts/
    build.ps1
    validate.ps1
```

The `docs/` directory keeps detailed release notes and implementation notes.
The `repo/` directory stores repository-facing screenshots and ordered demo walkthrough images used by the README.
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

Grade Guard is a keyboard-first Windows console application. The screenshots in `repo/demo/` below show a typical session from profile creation through grade review, using the actual prompts and menus from the CLI.

### Controls

- Use arrow keys to move between highlighted menu entries.
- Press `Enter` to confirm a menu choice or save a typed value.
- Press `Esc` to back out of selection screens or stop the initial add-course loop when prompted.
- Text-entry prompts commonly show `Backspace` when you want to clear and retype the current field.

### CLI Walkthrough

1. Create a new profile, confirm the captured student details, and add the first course.

   <p align="center">
     <img src="repo/demo/grade-guard-%20%281%29.png" alt="New profile confirmation screen" width="48%" />
     <img src="repo/demo/grade-guard-%20%282%29.png" alt="Initial add course screen after profile creation" width="48%" />
   </p>

   The first setup flow asks for the student's name, student number, and degree program. After confirmation, the CLI immediately asks for the first course name, unit count, and whether the course has a lab component.

2. Choose the target grade goal for the semester profile.

   <p align="center">
     <img src="repo/demo/grade-guard-%20%283%29.png" alt="Grade goal selection menu" width="70%" />
   </p>

   You can pick from preset goals such as `Passing`, `Average`, `Cum Laude`, `Magna Cum Laude`, and `Summa Cum Laude`, or enter a custom target. The demo selects `Summa Cum Laude [1.20]`.

3. Re-open the saved profile later through `Select Profile`.

   <p align="center">
     <img src="repo/demo/grade-guard-%20%284%29.png" alt="Select profile screen asking for student number" width="70%" />
   </p>

   After the profile has been saved to a local CSV, you can load it again by entering the same student number. This is the entry point for continuing work on an existing semester record.

4. Use the profile operations menu as the main hub for the rest of the workflow.

   <p align="center">
     <img src="repo/demo/grade-guard-%20%285%29.png" alt="Profile operations menu" width="70%" />
   </p>

   From here, you branch into `Activity Operations`, `Course Operations`, `Profile Settings`, `View Grades`, or `Logout`.

5. Define the grading breakdown for each course before adding scores.

   <p align="center">
     <img src="repo/demo/grade-guard-%20%286%29.png" alt="Course operations menu highlighting course parameters" width="48%" />
     <img src="repo/demo/grade-guard-%20%287%29.png" alt="Course parameter operations menu" width="48%" />
   </p>
   <p align="center">
     <img src="repo/demo/grade-guard-%20%288%29.png" alt="Prompt to enter a parameter name" width="48%" />
     <img src="repo/demo/grade-guard-%20%289%29.png" alt="Prompt to enter a parameter weight" width="48%" />
   </p>
   <p align="center">
     <img src="repo/demo/grade-guard-%20%2810%29.png" alt="Course parameter operations menu highlighting print parameters" width="48%" />
     <img src="repo/demo/grade-guard-%20%2811%29.png" alt="Printed parameter list for CMSC 18" width="48%" />
   </p>

   Open `Course Operations`, choose `Course Parameters`, and add weighted categories such as `Quiz` and `Exam`. The demo assigns `50%` to each and then uses `Print Parameters` to verify the course setup before moving on.

6. Update the grade goal later from `Profile Settings` if the target changes.

   <p align="center">
     <img src="repo/demo/grade-guard-%20%2812%29.png" alt="Profile settings menu" width="48%" />
     <img src="repo/demo/grade-guard-%20%2813%29.png" alt="Profile settings highlighting set grade goal" width="48%" />
   </p>

   This menu is the quick path for revising the semester goal without rebuilding the profile from scratch.

7. Register activities before entering earned points.

   <p align="center">
     <img src="repo/demo/grade-guard-%20%2814%29.png" alt="Activity operations menu highlighting add activity" width="48%" />
     <img src="repo/demo/grade-guard-%20%2815%29.png" alt="Add activity course selection screen" width="48%" />
   </p>
   <p align="center">
     <img src="repo/demo/grade-guard-%20%2816%29.png" alt="Add activity parameter selection screen" width="48%" />
     <img src="repo/demo/grade-guard-%20%2817%29.png" alt="Prompt to enter activity name and total score" width="48%" />
   </p>

   Go to `Activity Operations`, select the course, pick the parameter bucket, then enter the activity name and total score. The demo adds `Quiz 1` with a total score of `10`. Repeat the same loop for every quiz, long exam, project, or assignment you want to track.

8. Enter the achieved scores and watch the prediction update.

   <p align="center">
     <img src="repo/demo/grade-guard-%20%2818%29.png" alt="Activity operations menu highlighting add score" width="48%" />
     <img src="repo/demo/grade-guard-%20%2819%29.png" alt="Add score course selection screen" width="48%" />
   </p>
   <p align="center">
     <img src="repo/demo/grade-guard-%20%2820%29.png" alt="Add score parameter selection screen" width="32%" />
     <img src="repo/demo/grade-guard-%20%2821%29.png" alt="Add score activity selection screen" width="32%" />
     <img src="repo/demo/grade-guard-%20%2822%29.png" alt="Prompt to enter a new score" width="32%" />
   </p>

   Use `Add Score` to select the course, parameter, and exact activity, then type the score you earned. The demo records `48` for `Long Exam 2`, and the predicted GWA jumps from `4.28` to `1.32`, which the profile reports as `97.40%` of the `1.20` goal.

9. Open `View Grades` to inspect the full stored breakdown.

   <p align="center">
     <img src="repo/demo/grade-guard-%20%2823%29.png" alt="Profile operations menu highlighting view grades" width="48%" />
     <img src="repo/demo/grade-guard-%20%2824%29.png" alt="Detailed view grades output" width="48%" />
   </p>

   The final screen prints each course, component, parameter, and recorded activity with its saved score. The later demo screenshots show a fuller sample dataset after repeating the same `Add Activity` and `Add Score` loops for more quizzes and exams.

### Resume a Saved Profile

When you are done entering data, choose `Logout` or exit from the main menu. Grade Guard persists the profile locally, and you can continue later by returning to `Select Profile` and entering the student number shown during setup.

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
