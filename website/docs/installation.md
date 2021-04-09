---
title: Installation 
id: installation
slug: /installation
---

## Dependencies

Install `reform` and `reschema`:

```sh
yarn add @rescriptbr/reform @rescriptbr/reschema
```
> `reschema` is a schema library that reform uses to validate form fields. Check out the official [documentation](!https://reform.rescriptbr.org).


Then add `lenses-ppx`:

```sh
yarn add @rescriptbr/lenses-ppx@4.0.0 -D
```

> `lenses-ppx` is a preprocessor that you can use to generate type-safe "getters" and "setters" to the form values. Check out the official [documentation](!https://lenses-ppx.rescriptbr.org).

Then add these dependencies to `bsconfig.json`:
```diff
{
  ...
  "bs-dependencies": [
+    "@rescriptbr/reform",
+   "@rescriptbr/reschema"
  ],
  "ppx-flags": [
+    "@rescriptbr/lenses-ppx/ppx"
  ]
}
```

