# EOEngine

**EOEngine** is a modern, community-focused fork of [**Etheos**](https://github.com/ethanmoffat/etheos) — a high-performance emulator for **Endless Online** servers. This project is built with ease-of-use, modularity, and community involvement in mind.

Originally derived from [EOSource](https://www.eosource.net) — itself a continuation of [EOServ](https://github.com/Sausages/eoserv) — EOEngine brings together years of open-source development history with a renewed vision: to provide a powerful, flexible, and beginner-friendly server framework for hosting Endless Online experiences.

EOEngine will be continuously developed and maintained by members of the **Endless Online community**, and welcomes open contributions from all skill levels.

> 💡 **Credits:** This project would not be possible without the foundational work of Ethan Moffat and contributors to [Etheos](https://github.com/ethanmoffat/etheos). EOEngine inherits and builds upon Etheos's modern C++ architecture, tooling, and community-driven features.

---

## ✨ Key Objectives

- **Accessibility**: Make it easier for players, hobbyists, and developers to create and host their own Endless Online servers.
- **Customization**: Extend the emulator with new features, configuration options, and compatibility for public and private EO content.
- **Community-Centric Development**: Encourage contributions, shared learning, and plugin/module support.
- **Ongoing Maintenance**: Provide regular updates and modern development practices to keep EOEngine stable and evolving.

---

## 📚 Table of Contents

- [Getting Started on Windows](#getting-started-on-windows)
- [Getting Started on Linux](#getting-started-on-linux)
- [Docker Image](#docker-image)
- [Running the Server](#running-the-server)
- [Development Environment](#development-environment)
- [Integration Tests](#integration-tests)
- [Contributing](#contributing)
- [License](#license)

---

## 🖥️ Getting Started on Windows

EOEngine can be compiled using **Visual Studio 2017 or 2019**. Ensure the **"Desktop Development with C++"** workload is installed, along with the **Windows 10 SDK** for SQL ODBC support.

> ⚠️ Please uninstall MinGW if you’ve previously used it to build EOServ. It conflicts with Visual Studio's standard libraries.

### Clone the Repository

> Note: This project uses **Git LFS** for asset management. Do not download as ZIP.

```bash
git lfs install         # Run this once per system
git clone https://github.com/your-org-or-username/EOEngine.git
cd EOEngine
```

### Install Dependencies

Run the provided PowerShell script as an Administrator:

```powershell
.\scripts\install-deps.ps1
```

This will install:
- CMake (>= 2.8.2)
- SQLite
- MariaDB
- vswhere
- Git (for bcrypt/googletest)

### Build the Project

```powershell
.\build-windows.ps1 -SqlServer ON -MariaDB ON -Sqlite ON -Debug
```

- Omit `-Debug` to build in Release mode.
- The output will be placed in a local `/install` folder.

---

## 🐧 Getting Started on Linux

Tested on **Ubuntu 18.04**, **20.04**, and **22.04** (including WSL2).

### Install Dependencies

```bash
sudo ./scripts/install-deps.sh
```

Installs:
- `g++`
- `cmake`
- `sqlite`
- `mariadb`
- `git`
- Optional: `ODBC` for SQL Server

### Build and Install

```bash
./build-linux.sh -i
```

Compiled binaries and resources will be placed in the `install/` directory.

---

## 🐳 Docker Image

A Docker image for EOEngine will be available via the community Docker Hub account (coming soon). It supports full configuration via environment variables:

```bash
docker pull yourdockeruser/eoengine
docker run -it --rm -e 'EOENGINE_PORT=8079' -p 8079:8079 -v $(pwd)/install/data:/eoengine/data -v $(pwd)/install/config:/eoengine/config yourdockeruser/eoengine:latest
```

---

## 🚀 Running the Server

Ensure configuration files and data are in place (`install/config`, `install/data`).

```bash
cd install
./eoengine           # Linux
.\eoengine.exe       # Windows
```

---

## 🛠️ Development Environment

EOEngine is compatible with **Visual Studio Code** and modern C++ development workflows.

Recommended VSCode extensions:
- C/C++ (`ms-vscode.cpptools`)
- CMake (`twxs.cmake`)

Pull requests, bug fixes, and new feature contributions are always welcome.

---

## 🧪 Integration Tests

EOEngine supports integration testing using [EOBot](https://github.com/ethanmoffat/EndlessClient/tree/master/EOBot) from the EndlessClient repo. These tests simulate gameplay interactions and run inside Docker containers.

To run locally:

1. Install:
   - Python 3
   - Docker
   - Add user to Docker group (`sudo usermod -aG docker $USER`)
   - Download latest [EOBot](https://github.com/ethanmoffat/EndlessClient/releases)

2. Run test script:

```bash
cd deploy
./ci-test.sh --self-contained --use-local --botdir /path/to/eobot
```

---

Stay tuned on our Discord or project wiki for downloads and previews.

---

## 🤝 Contributing

EOEngine is built by the community, for the community. Whether you're:
- Fixing bugs
- Adding new features
- Writing documentation
- Testing new builds

...your contributions matter. See [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

---

## 📜 License & Credits

EOEngine is an open-source project under the **MIT License**.

We gratefully acknowledge:
- **Ethan Moffat** for [Etheos](https://github.com/ethanmoffat/etheos)
- The **EOSource** original developers [www.eosource.net](https://www.eosource.net)
- The **EOServ** project and early EO emulator pioneers
- All past and future contributors to the Endless Online engine ecosystem

