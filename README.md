# 📦 TextSim - Terminal Secure Chat

**TextSim** é um sistema de chat seguro via terminal, desenvolvido em **C++17**. Ele permite que o usuário atue como **cliente** ou **servidor** em uma rede local ou remota, utilizando **criptografia RSA** para proteger a troca de mensagens e autenticação. Ideal para estudantes, desenvolvedores e entusiastas que desejam aprender mais sobre redes, segurança e programação em C++.

> ⚠️ **Aviso:** ainda **não possui persistência de dados** e a **segurança será aprimorada** em versões futuras.

---

## 🚀 Funcionalidades

- 🔐 **Criptografia RSA** com validação por **SHA-256**
- 🧩 **Modo Servidor**: escuta conexões, troca chaves e recebe mensagens
- 🔌 **Modo Cliente**: conecta a servidores, realiza handshake seguro e envia mensagens
- 🧠 **Geração de chaves RSA** automática ou manual
- 🎨 **Interface de menu interativa** com estilos ANSI no terminal
- ⚙️ **Configuração de portas, nome de usuário e status da conexão**

---

## 📚 Tecnologias Utilizadas

- **C++17** com **CMake**
- [GMP](https://gmplib.org/) – operações com grandes números (RSA)
- [httplib](https://github.com/yhirose/cpp-httplib) – comunicação HTTP
- [nlohmann::json](https://github.com/nlohmann/json) – serialização de dados JSON
- Estilização via ANSI Escape Codes (VT100)

---

## 🧪 Requisitos

- **Windows 10 ou superior**
- **Microsoft Visual C++ Redistributable 2015–2022**  
  🔗 [Baixar vc_redist.x64.exe](https://aka.ms/vs/17/release/vc_redist.x64.exe)

---

## 📥 Conteúdo da Release

- `TextSim.exe` – Aplicativo principal
- `gmp-10.dll` – Biblioteca GMP
- `brotlienc.dll`, `brotlidec.dll` – Compressão Brotli usada por dependências
- `README.txt` – Guia básico de uso

---

## ✅ Como usar

1. Extraia os arquivos do `.zip`
2. Execute `TextSim.exe`
3. Use o menu para iniciar o servidor ou conectar-se a outro
4. Aproveite um chat seguro via terminal!

---

## 👨‍💻 Desenvolvido por

**Ícaro Gabriel Andrade Santos**  
Estudante de Ciência da Computação e entusiasta de redes, criptografia e desenvolvimento de sistemas.
