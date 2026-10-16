# raylib-art

raylib으로 제너러티브 아트와 셰이더 아트를 만드는 C 프로젝트입니다. Windows 빌드에 필요한 raylib 소스, 컴파일러, 디버거를 프로젝트 안에 보관합니다. 

## 폴더 구조

```text
raylib-art/
├─ src/                      실행할 프로그램 (main.c)
├─ include/                  프로젝트에서 작성한 헤더
├─ artwork_code/             작품별 소스 보관
│  ├─ generative_art/        :: 일반 아트워크
│  │  ├─ 01_rotating_square.c
│  │  ├─ 02_wave_lines.c
│  │  ├─ 03_particle.c
│  │  ├─ 04_color_array.c
│  │  └─ 05_attractor.c
│  └─ shader_art/            :: 셰이더 아트워크
│     └─ 01_shader_basic/
│        ├─ shader_basic.c
│        └─ shaders/
│           ├─ vert.glsl
│           └─ frag.glsl
├─ resources/                이미지, 폰트, 오디오
├─ third_party/
│  └─ raylib/                raylib 전체 소스와 라이선스
├─ tools/
│  ├─ build.ps1              Windows 빌드 진입점
│  ├─ premake/               빌드 설정, 생성 도구, ECC 모듈
│  └─ w64devkit/             GCC, Make, GDB 및 표준 라이브러리
├─ build/                    생성된 Makefile, 오브젝트, 컴파일 정보
├─ bin/Debug/                Debug 실행 파일과 raylib 라이브러리
├─ bin/Release/              Release 실행 파일과 raylib 라이브러리
├─ .vscode/                  IntelliSense, 빌드, 디버깅 설정
└─ Makefile                  공통 빌드 진입점
```

## VS Code에서 사용

1. 이 프로젝트 폴더를 열고 권장 확장인 Microsoft C/C++를 설치합니다.
2. **Ctrl+Shift+B**: Debug 빌드.
3. **F5**: Debug 빌드 후 GDB로 디버깅. 실행 중인 창을 닫으면 종료됩니다.
4. Release는 작업 실행 메뉴의 **build release**, 또는 실행 구성의 **Run Release**를 선택합니다.

구성을 변경한 직후에는 VS Code 창을 다시 로드하고 새 터미널을 여세요. 헤더 정의로 이동하면 `third_party/raylib/src/raylib.h`가 열립니다.

## Windows 명령줄

**평소에는 `make`를 사용하면 됩니다.** VS Code에서 이 프로젝트를 열고 **새 터미널**을 만들면 `.vscode/settings.json`이 프로젝트의 `tools/w64devkit/bin`을 PATH에 추가합니다. 아래 명령은 모두 프로젝트 루트(`C:\Works\Dev\raylib-art`)에서 실행합니다.

| 작업 | 명령 |
| --- | --- |
| Debug 빌드 | `make` |
| Debug 병렬 빌드 | `make -j8` |
| Debug 실행 | `.\bin\Debug\raylib-art.exe` |
| Release 빌드 | `make config=release_x64 -j8` |
| Release 실행 | `.\bin\Release\raylib-art.exe` |
| Debug 결과 정리 | `make clean` |
| Release 결과 정리 | `make config=release_x64 clean` |
| Debug 전체 재컴파일 | `make -B -j8` |
| Release 전체 재컴파일 | `make config=release_x64 -B -j8` |

`make`의 기본 설정은 `debug_x64`입니다. `-j8`은 최대 8개 작업을 병렬로 실행하는 옵션입니다. `clean`은 선택한 설정의 실행 파일, raylib 라이브러리, 오브젝트를 제거합니다. Debug와 Release를 모두 정리하려면 두 clean 명령을 각각 실행하세요. 작품 소스와 리소스는 유지됩니다.

빌드 후 실행하려면 다음 두 줄을 순서대로 실행합니다. 빌드가 성공했는지 확인한 뒤 실행하세요.

```powershell
make -j8
.\bin\Debug\raylib-art.exe
```

### make를 찾을 수 없을 때

기존 터미널을 닫고 VS Code에서 새 터미널을 여세요. VS Code 밖의 PowerShell에서 작업한다면, 프로젝트 루트에서 다음 명령으로 **현재 터미널에만** 로컬 도구 경로를 추가할 수 있습니다.

```powershell
$env:PATH = "$((Get-Location).Path)\tools\w64devkit\bin;$env:PATH"
make -j8
```

전역 PATH나 시스템의 스크립트 실행 정책을 변경할 필요는 없습니다.

### 배치 파일과 make의 관계

`build-MinGW-W64.bat`도 유효합니다. 이 파일은 `tools/build.ps1`을 통해 로컬 도구 경로를 설정하고, 같은 Makefile을 호출합니다. VS Code 밖에서 PATH 설정 없이 빌드하거나 빌드 후 바로 실행할 때 사용할 수 있습니다.

| make 명령 | 배치 파일의 대응 명령 |
| --- | --- |
| `make -j8` | `.\build-MinGW-W64.bat` |
| `make config=release_x64 -j8` | `.\build-MinGW-W64.bat -Configuration Release` |
| `make clean` | `.\build-MinGW-W64.bat -Clean` |
| `make config=release_x64 clean` | `.\build-MinGW-W64.bat -Configuration Release -Clean` |
| `make -B -j8` | `.\build-MinGW-W64.bat -Rebuild` |

배치 파일의 기본값도 Debug x64이며, 병렬 작업 수는 CPU 수에 따라 최대 8개입니다. `-Jobs 4`로 조절하고, `-Run`을 추가하면 빌드 성공 후 실행합니다. 배치 파일은 어느 작업 디렉터리에서 호출해도 프로젝트 루트를 기준으로 동작합니다. VS Code의 빌드 작업 역시 이 스크립트를 사용합니다.

두 방식 모두 빌드 설정과 `build/compile_commands.json`을 현재 경로에 맞춰 생성한 뒤 빌드합니다.

## 작품 작업 방법

빌드 대상은 `src/`의 소스입니다. `artwork_code/`는 작품을 보관하는 폴더이므로 여러 작품의 `main()`이 함께 링크되지 않습니다. 다른 작품을 실행하려면 현재 `src/main.c`를 먼저 보관하고 원하는 작품을 `src/main.c`에 복사한 뒤 빌드합니다.

- `artwork_code/generative_art/`: 제너러티브 아트의 C 소스를 보관합니다. 예를 들어 `rotating_square.c`를 `src/main.c`에 복사해 실행합니다.
- `artwork_code/shader_art/`: 작품마다 하위 폴더를 만들고 C 코드와 해당 작품의 `shaders/`를 함께 보관합니다.

셰이더 기본 작품은 `artwork_code/shader_art/01_shader_basic/shader_basic.c`의 내용을 `src/main.c`에 복사한 뒤 **F5**로 실행합니다. 이 코드의 `LoadShader()`는 이미 프로젝트 루트를 기준으로 다음 경로를 사용하므로, 셰이더 파일은 작품 폴더에 그대로 두면 됩니다.

```c
LoadShader(
    "artwork_code/shader_art/01_shader_basic/shaders/vert.glsl",
    "artwork_code/shader_art/01_shader_basic/shaders/frag.glsl");
```

프로그램의 작업 디렉터리는 프로젝트 루트입니다. `"shaders/frag.glsl"`은 루트의 `shaders/`를 가리키며, 작품 폴더 안의 `shaders/`를 자동으로 찾지 않습니다. 새 작품을 추가하거나 작품 폴더를 옮길 때는 `LoadShader()`와 다른 리소스 로딩 경로도 프로젝트 루트 기준으로 맞추세요.

## 의존성과 백업

- raylib은 기존에 빌드하던 **6.1-dev 소스 스냅샷**을 그대로 사용합니다. 빌드 중 다운로드하거나 자동으로 업데이트하지 않습니다. 출처와 식별 정보는 [third_party/README.md](third_party/README.md)에 있습니다.
- `third_party/raylib/`은 소스와 함께 버전 관리할 수 있도록 프로젝트의 ignore 규칙에서 제외하지 않았습니다. 라이브러리 업데이트 시 소스 전체를 함께 교체하고 재빌드하세요.
- `tools/w64devkit/`은 **2.7.0 / GCC 15.2.0**입니다. 크기가 약 561 MB이므로 Git에서는 제외합니다. 현재 폴더를 통째로 백업하면 함께 보관됩니다. Git으로만 다른 PC에 옮겼다면 동일한 w64devkit 배포본 전체를 이 위치에 별도로 복원하세요. 실행 파일 몇 개만 복사하면 표준 헤더와 라이브러리가 누락됩니다.
- `build/`와 `bin/`은 생성 결과물이므로 Git에서 제외합니다. `build/`는 삭제해도 다음 빌드에서 생성됩니다.

## 다른 빌드 도구

Visual Studio 프로젝트 생성은 기존 `build-VisualStudio2022.bat`, `build-VisualStudio2026.bat`을 사용합니다. 생성된 솔루션은 `build/`에 있습니다. 해당 Visual Studio C++ 개발 도구가 별도로 필요합니다.

Linux/macOS에서는 해당 OS의 C/C++ 컴파일러와 Make가 필요합니다. 프로젝트 루트에서 `make config=debug_x64`를 실행하며, Apple Silicon은 `make config=debug_arm64`를 사용합니다. 생성 도구 실행 권한이 없다면 `chmod +x tools/premake/premake5 tools/premake/premake5.osx`로 부여합니다. Linux에는 GLFW가 요구하는 시스템 개발 패키지가 필요합니다. 이번 구성의 실제 빌드 검증 환경은 Windows x64입니다.

## 출처

Raylib-Quickstart by Jeffery Myers를 바탕으로 합니다. 원본 템플릿은 [CC0 1.0](https://creativecommons.org/publicdomain/zero/1.0/)입니다. raylib과 도구의 라이선스는 각 배포 폴더에 보존되어 있습니다.
