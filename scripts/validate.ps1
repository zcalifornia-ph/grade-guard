param(
    [string]$OutputDir = "artifacts/validation"
)

$ErrorActionPreference = "Stop"

$ProjectRoot = Resolve-Path (Join-Path $PSScriptRoot "..")
Set-Location $ProjectRoot

New-Item -ItemType Directory -Force -Path $OutputDir | Out-Null

$WarningFlags = @(
    "-std=c17",
    "-Wall",
    "-Wextra",
    "-pedantic"
)

function Invoke-GccBuild {
    param(
        [string]$OutputPath,
        [string[]]$Sources
    )

    $Arguments = $WarningFlags + @(
        "-I",
        "grade-guard/header"
    ) + $Sources + @(
        "-o",
        $OutputPath
    )

    & gcc @Arguments
    if ($LASTEXITCODE -ne 0) {
        throw "Build failed for $OutputPath"
    }
}

function Invoke-Executable {
    param(
        [string]$ExecutablePath
    )

    & $ExecutablePath
    if ($LASTEXITCODE -ne 0) {
        throw "Execution failed for $ExecutablePath"
    }
}

$AppOutput = Join-Path $OutputDir "grade-guard.exe"
& (Join-Path $PSScriptRoot "build.ps1") -OutputPath $AppOutput

$Targets = @(
    @{
        Name = "vector_test"
        Sources = @(
            "grade-guard/tests/vector_test.c",
            "grade-guard/source/vector.c"
        )
    },
    @{
        Name = "models_lifecycle_test"
        Sources = @(
            "grade-guard/unit-tests/models_lifecycle_test.c",
            "grade-guard/source/models.c",
            "grade-guard/source/vector.c"
        )
    },
    @{
        Name = "persistence_contract_test"
        Sources = @(
            "grade-guard/unit-tests/persistence_contract_test.c",
            "grade-guard/source/persistence.c",
            "grade-guard/source/models.c",
            "grade-guard/source/vector.c"
        )
    },
    @{
        Name = "unit5_defect_baseline_test"
        Sources = @(
            "grade-guard/unit-tests/unit5_defect_baseline_test.c",
            "grade-guard/source/grade_calc.c",
            "grade-guard/source/models.c",
            "grade-guard/source/vector.c",
            "grade-guard/source/ui_console.c"
        )
    },
    @{
        Name = "app_smoke_test"
        Sources = @(
            "grade-guard/unit-tests/app_smoke_test.c",
            "grade-guard/source/app.c",
            "grade-guard/source/persistence.c",
            "grade-guard/source/models.c",
            "grade-guard/source/vector.c",
            "grade-guard/source/grade_calc.c"
        )
    }
)

foreach ($Target in $Targets) {
    $ExecutablePath = Join-Path $OutputDir ($Target.Name + ".exe")
    Invoke-GccBuild -OutputPath $ExecutablePath -Sources $Target.Sources
    Invoke-Executable -ExecutablePath $ExecutablePath
}

Write-Host "Validation pipeline passed."
Write-Host "Generated CSV/runtime artifacts remain under grade-guard/unit-tests for manual cleanup."
