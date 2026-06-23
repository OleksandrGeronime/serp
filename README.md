# SERP

**SERP** (Service · Event · Runtime · Platform) — a C++ framework for building distributed, multiprocess embedded and automotive systems.

SERP turns a high-level service specification into a generated skeleton — services, interfaces, transports — that compiles and runs out of the box. You fill in the business logic; the framework handles threading, IPC, watchdogs, and lifecycle.

## Packages

| Package | Contents |
|---|---|
| `serp` | Runtime shared libraries (deploy your app) |
| `serp-dev` | Headers, CMake config, engineering libs, `serpgen` code generator |

---

## macOS

### Upgrade (if already installed)

```bash
brew update && brew upgrade OleksandrGeronime/serp/serp-dev
```

### One-liner install (recommended)

```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/OleksandrGeronime/homebrew-serp/main/install.sh)"
```

Installs `serp-dev` (includes everything). Runtime-only:

```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/OleksandrGeronime/homebrew-serp/main/install.sh)" -- serp
```

### Manual install (Homebrew)

```bash
brew tap OleksandrGeronime/serp
brew install OleksandrGeronime/serp/serp-dev   # or: serp
```

---

## Ubuntu / Debian

### Upgrade (if already installed)

Download the new `.deb` from [Releases](https://github.com/OleksandrGeronime/serp/releases/latest) and re-run `dpkg -i`.

### Manual install

Download the latest `.deb` packages from the [Releases](https://github.com/OleksandrGeronime/serp/releases/latest) page and install:

```bash
# Replace x.y.z with the version you want
VERSION=0.5.0

wget https://github.com/OleksandrGeronime/serp/releases/download/v${VERSION}/serp_${VERSION}_amd64.deb
wget https://github.com/OleksandrGeronime/serp/releases/download/v${VERSION}/serp-dev_${VERSION}_amd64.deb

sudo dpkg -i serp_${VERSION}_amd64.deb serp-dev_${VERSION}_amd64.deb
```

---

## Verify

```bash
serpgen --version
```

---

## CMake integration

After installing `serp-dev`:

```cmake
find_package(Serp REQUIRED)
target_link_libraries(your_target Serp::core Serp::transport Serp::logger_console)
```

---

## Demo

See [serp-demo](https://github.com/OleksandrGeronime/serp-demo) for hands-on examples of every framework primitive.

---

## License

SERP is distributed in binary form under the
[Polyform Noncommercial License 1.0.0](LICENSE) —
free for noncommercial use.

Commercial use requires a separate license.
See [LICENSE-COMMERCIAL.md](LICENSE-COMMERCIAL.md) for details.
