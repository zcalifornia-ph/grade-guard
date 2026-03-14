param(
    [string]$OutputPath = "artifacts/build/grade-guard.exe"
)

$ErrorActionPreference = "Stop"

$ProjectRoot = Resolve-Path (Join-Path $PSScriptRoot "..")
Set-Location $ProjectRoot

$OutputDirectory = Split-Path -Parent $OutputPath
if ($OutputDirectory) {
    New-Item -ItemType Directory -Force -Path $OutputDirectory | Out-Null
}

$WarningFlags = @(
    "-std=c17",
    "-Wall",
    "-Wextra",
    "-pedantic"
)

$AppSources = @(
    "grade-guard/main.c",
    "grade-guard/source/app.c",
    "grade-guard/source/vector.c",
    "grade-guard/source/models.c",
    "grade-guard/source/grade_calc.c",
    "grade-guard/source/persistence.c",
    "grade-guard/source/ui_console.c",
    "grade-guard/source/profile_controller.c"
)

$Arguments = $WarningFlags + @(
    "-I",
    "grade-guard/header"
) + $AppSources + @(
    "-o",
    $OutputPath
)

& gcc @Arguments
if ($LASTEXITCODE -ne 0) {
    throw "Grade Guard application build failed."
}

Write-Host "Built $OutputPath"
