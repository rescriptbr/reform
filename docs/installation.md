---
id: installation
title: Installation
sidebar_label: Installation
---

### Pre-requisites

#### BuckleScript
 In order to start using Reform you need the basic tooling for ReasonML using [BuckleScript](https://bucklescript.github.io/en/).

#### React
Reform is a library for React applications, so we assume that you are already familiar with [React](https://reactjs.org/).

### Instalation and Setup

You’ll need to have [Node 8.16.0 or Node 10.16.0 or later version](https://nodejs.org/en/) on your local development machine (but it’s not required on the server). You can use `fnm`, `nvm` (macOS/Linux) or `nvm-windows` to easily switch Node versions between different projects.

If you want to create a ReasonReact development envinroment you can use the create-react-app to bootstrap a initial project.
`yarn create react-app my-app --scripts-version reason-scripts`
`yarn create` is available in [Yarn 0.25+](https://yarnpkg.com/lang/en/)

or

`bsb -init reform-demo -theme react-hooks`
`bsb` comes with BuckleScript installation

It will create a directory called my-app inside the current folder.
Inside that directory, it will generate the initial project structure.

Now you can install the package Reform to your existing project using yarn or npm:
```
yarn add bs-reform reschema
```

Then add to your `bs-dependencies` in bsconfig.json
```
"bs-dependencies": [
  "bs-reform",
  "reschema"
]
```

Now add lenses, a package to generate GADT lenses for ReasonML.
```
yarn add -D lenses-ppx
```

And update your `bs-dependencies`
```
"ppx-flags": [
  "lenses-ppx/ppx"
]
```
