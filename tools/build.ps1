param(
    [ValidateSet('Debug', 'Release')]
    [string]$Configuration = 'Debug',
    [ValidateSet('x64', 'x86')]
    [string]$Platform = 'x64',
    [ValidateRange(1, 64)]
    [int]$Jobs = [Math]::Max(1, [Math]::Min(8, [Environment]::ProcessorCount)),
    [switch]$Clean,
    [switch]$GenerateOnly,
    [switch]$Rebuild,
    [switch]$Run
)

$ErrorActionPreference = 'Stop'
$projectRoot = Split-Path -Parent $PSScriptRoot
$toolchainBin = Join-Path $PSScriptRoot 'w64devkit/bin'
$makeExecutable = Join-Path $toolchainBin 'mingw32-make.exe'
if (-not (Test-Path -LiteralPath $makeExecutable)) {
    throw 'Restore the complete w64devkit toolchain under tools/w64devkit before building.'
}
if (($Clean -and ($GenerateOnly -or $Rebuild -or $Run)) -or ($GenerateOnly -and ($Rebuild -or $Run))) {
    throw 'Use -Clean or -GenerateOnly separately from build/run options.'
}

$previousPath = $env:PATH
Push-Location -LiteralPath $projectRoot
try {
    $env:PATH = "$toolchainBin;$previousPath"
    $makeArguments = @('--no-print-directory', "config=$($Configuration.ToLowerInvariant())_$Platform", "-j$Jobs")
    if ($Clean) { $makeArguments += 'clean' }
    elseif ($GenerateOnly) { $makeArguments += 'generate' }
    if ($Rebuild) { $makeArguments += '-B' }

    & $makeExecutable @makeArguments
    if ($LASTEXITCODE -ne 0) { throw "Build failed (exit code $LASTEXITCODE)." }
    if ($Run) {
        & (Join-Path $projectRoot "bin/$Configuration/raylib-art.exe")
        if ($LASTEXITCODE -ne 0) { throw "Application exited with code $LASTEXITCODE." }
    }
}
finally {
    Pop-Location
    $env:PATH = $previousPath
}
