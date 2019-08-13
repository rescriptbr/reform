---
id: installation
title: Installation
sidebar_label: Installation
---

Add via yarn
```
yarn add bs-reform reschema
```
Then add to your bs-dependencies in bsconfig.json
```
"bs-dependencies": [
  "bs-reform",
  "reschema"
]
```

Now add lenses
```
yarn add -D lenses-ppx
```

And update your bs-dependencies
```
"ppx-flags": [
  "lenses-ppx/ppx"
]
```
