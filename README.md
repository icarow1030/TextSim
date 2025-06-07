# 📦 TextSim - A Secure Chat & Educational Tool

**TextSim** is a secure P2P chat system designed as a powerful educational tool to demonstrate cryptography and networking concepts in action. What started as a terminal-based application in **C++17** is evolving into a full-featured desktop application with a modern user interface built with **React and Tauri**.

The core mission of this project is to demystify how public-key cryptography (RSA) and hash-based integrity checks (SHA-256) work in a real-world scenario. Users can visualize the entire communication lifecycle, from key generation and handshake to the end-to-end encryption of messages.

> ⚠️ **Not every feature is working. Project is under development.**

---

### 📊 Project Status

This repository currently contains two main artifacts, which you can find on the **[Releases Page](https://github.com/icarow1030/TextSim/releases)**:

1.  **`v0.1.1-TerminalTestVersion` (Stable)**
    * A fully functional, command-line-only chat application written entirely in C++. It includes all the core cryptographic and networking logic.

2.  **`v0.1.0-alpha` (UI Prototype)**
    * A modern desktop application featuring the new user interface. This version is a **visual and interactive prototype** to showcase the final design. The C++ backend is not yet integrated.

---

### 🚀 Features

#### Core Logic (C++ Backend)
- 🔐 **RSA Encryption:** End-to-end encryption for message confidentiality, implemented with the GMP library for large number arithmetic.
- ✍️ **SHA-256 Validation:** Hash-based signatures to ensure message integrity and authenticity.
- 🧩 **P2P Architecture:** A client-server model where each instance of the application can act as both a client and a server, communicating directly with another instance.
- 🤝 **Secure Handshake:** A custom handshake protocol for the secure exchange of public keys between users.

#### Desktop UI Prototype (React + Tauri)
- 🖥️ **Modern Desktop UI:** A clean and intuitive user interface built with React, TypeScript, and Vite, packaged into a lightweight native application with Tauri.
- 🎓 **Interactive Log Viewer:** A real-time log that explains technical events (key generation, handshake, encryption) in easy-to-understand language.
- 📄 **PDF Report Generation:** An educational feature to export the session logs into a detailed, formatted PDF report.
- ⚙️ **Visual Settings:** User-friendly screens to configure network ports, usernames, and manage application state.

---

### 📚 Tech Stack

* **Core Backend:** C++17, CMake, [GMP](https://gmplib.org/), [httplib](https://github.com/yhirose/cpp-httplib), [nlohmann::json](https://github.com/nlohmann/json)
* **Desktop UI & Frontend:** React, TypeScript, Vite, jsPDF
* **Desktop App Framework:** [Tauri](https://tauri.app/)

---

### ✅ Getting Started

#### Testing the Terminal Version (`v0.1.1-TerminalTestVersion`)
1.  Go to the [Releases page](https://github.com/icarow1030/TextSim/releases) and download the `.zip` file for the terminal version.
2.  Extract the files.
3.  Run `TextSim.exe` in two separate terminal windows.
4.  Follow the in-app menu to configure one instance as a server and the other as a client to connect to it.

#### Exploring the UI Prototype (`v0.1.0-alpha`)
1.  Go to the [Releases page](https://github.com/icarow1030/TextSim/releases) and download the installer for your OS (e.g., `.msi` for Windows).
2.  Install and run the application.
3.  Navigate through the different screens to explore the interface. Note that all functionality is for demonstration purposes only in this version.

---

###  roadmap Future Work: The Integration

The next major milestone for this project is the full integration of the C++ backend with the new Tauri desktop UI. This will be achieved using **Tauri's sidecar pattern**, where the native desktop shell will manage and communicate with the C++ executable in the background. This will bring all the core cryptographic features to the modern user interface, completing the project's vision.

---

### 👤 Author

Created by **Ícaro Gabriel (icarow1030)**.

[![github](https://img.shields.io/badge/GitHub-181717?style=for-the-badge&logo=github&logoColor=white)](https://github.com/icarow1030)
