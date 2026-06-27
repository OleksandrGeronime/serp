# SERP

**SERP** (Service · Event · Runtime · Platform) — a C++ framework for building distributed, multiprocess systems.

SERP turns a high-level service specification into a generated skeleton — services, interfaces, transports — that compiles and runs out of the box. You fill in the business logic; the framework handles threading, IPC, RPC, watchdogs, and lifecycle.

## Packages

| Package | Contents |
|---|---|
| `serp` | Runtime shared libraries (deploy your app) |
| `serp-dev` | Headers, CMake config, engineering libs, `serpgen` code generator |

---

## macOS

### Homebrew (recommended)

```bash
brew tap OleksandrGeronime/serp
brew trust OleksandrGeronime/serp
brew install OleksandrGeronime/serp/serp-dev   # or: serp
```

### Upgrade (if already installed)

```bash
brew update && brew upgrade OleksandrGeronime/serp/serp-dev
```

---

## Ubuntu / Debian

### Manual install

Download the latest `.deb` packages from the [Releases](https://github.com/OleksandrGeronime/serp/releases/latest) page and install:

```bash
# Replace x.y.z with the version you want
VERSION=0.5.0

wget https://github.com/OleksandrGeronime/serp/releases/download/v${VERSION}/serp_${VERSION}_amd64.deb
wget https://github.com/OleksandrGeronime/serp/releases/download/v${VERSION}/serp-dev_${VERSION}_amd64.deb

sudo dpkg -i serp_${VERSION}_amd64.deb serp-dev_${VERSION}_amd64.deb
```

### Upgrade (if already installed)

Download the new `.deb` from [Releases](https://github.com/OleksandrGeronime/serp/releases/latest) and re-run `dpkg -i`.

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

- [serp-demo](https://github.com/OleksandrGeronime/serp-demo) — hands-on examples of every framework primitive.

- [serp-mini-ivi](https://github.com/OleksandrGeronime/serp-mini-ivi) — a full Mini IVI (In-Vehicle Infotainment) middleware stack built on SERP. Covers climate, media, navigation, phone, notifications, and vehicle state — all generated from declarative specs and running as a multiprocess system over DBus or gRPC. Includes a terminal HMI frontend and a Qt Quick graphical frontend. Three deployment variants: monolith, multiprocess-dbus, and multiprocess-dbus-qtquick.

---

## VS Code Extension

<img src="https://raw.githubusercontent.com/OleksandrGeronime/serp/main/docs/plugin-logo.png" width="64"/>

**SERP** — Visual Studio Code extension for SERP IVI projects. Provides an architecture explorer, live deployment control, runtime inspector, and AI sessions scoped to every part of your system.

| Feature | Description |
|---|---|
| **Explorer** | Browse deployments, services, spec files, and source files in one tree. Validate and regenerate directly from the panel. |
| **Active Deployment** | Build, run, and stop the active deployment. Shows processes, the services they host, and direct links to impl and generated files. |
| **Architecture Dashboard** | Interactive live view — process cards with live CPU/MEM stats, one-click access to sessions and impl files. |
| **Runtime Inspector** | Call methods with typed inputs, read and watch properties, subscribe to notifications — directly on the running process, no client code needed. Works over gRPC or D-Bus. |
| **SERP AI** | Scoped Claude Code sessions per role (Architecture · Service · HMI · Debug). Each session loads its own responsibility, allowed writes, and context from the MCP server. A context guard hook keeps Claude current; a scope guard enforces write restrictions on every edit. |

**Keyboard shortcuts:**

| Action | macOS | Windows / Linux |
|---|---|---|
| Validate specs | `Ctrl+Option+V` | `Ctrl+Alt+V` |
| Generate | `Ctrl+Option+G` | `Ctrl+Alt+G` |
| Architecture Dashboard | `Ctrl+Option+D` | `Ctrl+Alt+D` |
| Build | `Ctrl+Option+B` | `Ctrl+Alt+B` |
| Run | `Ctrl+Option+R` | `Ctrl+Alt+R` |
| Stop | `Ctrl+Option+S` | `Ctrl+Alt+S` |

---

## License

SERP is distributed in binary form under the
[Polyform Noncommercial License 1.0.0](LICENSE) —
free for noncommercial use.

Commercial use requires a separate license.
See [LICENSE-COMMERCIAL.md](LICENSE-COMMERCIAL.md) for details.
