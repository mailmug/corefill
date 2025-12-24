# Corefill

**Corefill** is a high-performance **C-based polyfill extension for PHP**, designed to provide missing core functions, language features, and standard library behavior across different PHP versions — **implemented natively in C for speed, consistency, and lower memory overhead**.

Unlike userland polyfills written in PHP, Corefill operates at the **Zend Engine level**, offering near-native performance and deeper integration with PHP internals.

---

## Why Corefill?

Popular polyfill libraries (such as Symfony Polyfills) are written in PHP. While extremely useful, they:

- Add runtime overhead
- Increase memory usage
- Cannot hook deeply into Zend internals
- Behave differently than true core implementations

**Corefill solves this by implementing polyfills directly in C.**

### Key Advantages

- ⚡ Native performance  
- 🧠 Lower memory usage  
- 🔧 Zend Engine–level integration  
- 🔄 Consistent behavior across PHP versions  
- 🧩 Drop-in replacement for missing core features  

---

## Goals

- Provide **missing PHP core functions** in older PHP versions
- Backport **newer PHP features** where feasible
- Offer **consistent behavior** across 8.x, and beyond
- Act as a **foundation extension** for other C extensions
- Reduce dependency on heavy userland polyfills

---

## What Can Be Polyfilled?

Corefill focuses on features that are:

- Frequently polyfilled in PHP userland
- Performance-critical
- Safe to implement in C

### Examples

- Missing core functions
- String helpers
- Array helpers
- Hash / crypto helpers
- Language behavior adjustments
- Internal constants
- Engine quirks across versions


---

## Supported PHP Versions

| PHP Version | Status |
|------------|--------|
| PHP 8.0+   | ✅ Supported |
| PHP 8.1+   | ✅ Supported |
| PHP 8.2+   | ✅ Supported |
| PHP 8.3+   | ✅ Supported |
| PHP 8.4+   | ✅ Supported |
| PHP 8.5+   | ✅ Supported |

---

## Installation

```bash
pie install mailmug/corefill
```

### From Source

```bash
git clone https://github.com/yourname/corefill.git
cd corefill
phpize
./configure
make
sudo make install
