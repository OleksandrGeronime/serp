# SERP

**SERP** (Service · Event · Runtime · Platform) — a C++ framework for building large distributed, multiprocess systems with AI.

SERP turns a high-level service specification into a generated skeleton — services, interfaces, transports — that compiles and runs out of the box. You fill in the business logic; the framework handles threading, IPC, RPC, watchdogs, and lifecycle. That same boundary is what scopes AI work: the [VS Code extension](#vs-code-extension) turns it into role-scoped AI sessions, durable design contracts, and batch implementation runs that can build out a whole system's services at once.

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

Download `.deb` packages from [Releases](https://github.com/OleksandrGeronime/serp/releases/latest) and install:

```bash
sudo dpkg -i serp_*.deb serp-dev_*.deb
```

Full instructions: [Installation](https://github.com/OleksandrGeronime/serp/wiki/Installation)

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
| **Explorer** | Browse specs, deployments (build/run/stop, processes, hosted services), and services (interfaces, sequences, tests, generated + impl files) in one tree. Validate and regenerate directly from the panel. |
| **Architecture Dashboard** | Interactive live view — process cards with live CPU/MEM stats, one-click access to sessions and impl files. |
| **Runtime Inspector** | Call methods with typed inputs, read and watch properties, subscribe to notifications — directly on the running process, no client code needed. Works over gRPC or D-Bus. |
| **SERP AI Sessions** | Scoped agent sessions — architecture, service, HMI, sequence, debug/fix, freeform exploration — on a pluggable backend (Claude Code or Codex). A context guard hook keeps the agent current; a scope guard enforces write restrictions on every edit. |
| **Design Contracts** | Durable, user-approved design blocks that gate what an AI session may call "implemented," with a sync path for projects adopting it after the fact. |
| **SERP AI Runs** | Batch-plans and spawns implementation sessions for every design block that isn't implemented yet, project-wide, as one resumable job — this is how SERP scales AI-driven development across many services at once. |

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

## Documentation

Full documentation is available in the [Wiki](https://github.com/OleksandrGeronime/serp/wiki):

- [Quick Start](https://github.com/OleksandrGeronime/serp/wiki/Quick-Start)
- [Core Concepts](https://github.com/OleksandrGeronime/serp/wiki/Core-Concepts)
- [SerpIDL — Spec Format](https://github.com/OleksandrGeronime/serp/wiki/SerpIDL-Overview)
- [Code Generator](https://github.com/OleksandrGeronime/serp/wiki/Generator-Overview)
- [VS Code Plugin](https://github.com/OleksandrGeronime/serp/wiki/Plugin-Overview)
- [AI-Driven Development](https://github.com/OleksandrGeronime/serp/wiki/AI-Driven-Development)
- [SERP AI Sessions](https://github.com/OleksandrGeronime/serp/wiki/SERP-AI-Sessions)
- [Design Contracts](https://github.com/OleksandrGeronime/serp/wiki/Design-Contracts)
- [Implementation Runs](https://github.com/OleksandrGeronime/serp/wiki/Implementation-Runs)

---

## License

SERP is distributed in binary form under the
[Polyform Noncommercial License 1.0.0](LICENSE) —
free for noncommercial use.

Commercial use requires a separate license.
See [LICENSE-COMMERCIAL.md](LICENSE-COMMERCIAL.md) for details.
