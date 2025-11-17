# lkmini
### A minimal utility toolkit extracted from the LILAK framework

**lkmini** is a lightweight collection of utility classes extracted from the **LILAK** framework.
It provides a few convenient tools that can be used independently without loading the full LILAK environment.

If you already use LILAK, you **do not need** lkmini — the full framework already contains all functionalities.
lkmini is intended for users who want a minimal setup or need only a few small tools without the full LILAK dependency.

---

## Build & Installation

### 1. Configure & build
```bash
mkdir build
cd build
cmake ..
make
```

### 2. Generated `rootlogon.C`
During the build process, a file named **`rootlogon.C`** will be created in your **home directory**.
Add the following line to your `~/.rootrc`:

```
Rint.Logon: ~/rootlogon.C
```

This enables ROOT to automatically load lkmini when starting an interactive ROOT session.
Note that you activate only one rootlogon.C at a time.

LILAK and lkmini both generate their own `rootlogon.C` files.
Therefore, you should use **only one** at a time — lkmini *or* LILAK — to avoid conflicts.
