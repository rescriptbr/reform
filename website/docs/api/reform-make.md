---
title: ReForm.Make
id: reform-make
slug: /reform-make
---

`ReForm.Make(...)` is a module functor that creates a new form module by using a lenses module. It expects a config module with the following signature:
```rescript
  type field<'a>
  type state
  let set: (state, field<'a>, 'a) => state
  let get: (state, field<'a>) => 'a
  ```
Usually you don't need to create this module manually. We recommend to use the [lenses-ppx](https://github.com/Astrocoders/lenses-ppx) to generate this module automatically. 
> You can read more about the usage of lenses-ppx with ReForm [here](/docs/getting-started#creating-our-form)

