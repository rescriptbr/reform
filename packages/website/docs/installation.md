---
title: Installation 
id: installation
slug: /installation
---

## Dependencies 📦

Install `reform` and `reschema`:

```sh
yarn add @rescriptbr/reform @rescriptbr/reschema
```
> `@rescriptbr/reschema` is a schema library that reform uses to validate form fields. Check out the official [documentation](https://github.com/rescriptbr/reschema).


Then add `lenses-ppx`:

```sh
yarn add lenses-ppx -D
```

> `lenses-ppx` is a preprocessor that you can use to generate type-safe "getters" and "setters" to the form values. Check out the official [documentation](https://github.com/Astrocoders/lenses-ppx).

Then add these dependencies to `bsconfig.json`:
```diff
{
  ...
  "bs-dependencies": [
+    "@rescriptbr/reform",
+    "@rescriptbr/reschema"
  ],
  "ppx-flags": [
+    "lenses-ppx/ppx"
  ]
}
```

