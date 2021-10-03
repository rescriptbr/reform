---
title: What this is and why
id: what-and-why
slug: /
---

## What is this? 🤔

As you might know, you can use any existing React / JavaScript libraries with ReScript, 
including **form libraries** like Formik, react-hook-form, Final Form, since you install or create bindings for these libraries.
ReForm is not a set of bindings for an existing form library, it was created from scratch to use with ReScript and React. 


## Why? 🧐

Code that deals with strongly typed forms can quickly become walls of repeated text. 
We created ReForm to be both deadly simple and to make forms sound good leveraging ReScript's powerful typesytem. 
Even the schemas we use are nothing more than constructors built-in in the language itself with a small size footprint.

## Features 📦

- Hook API
- Schema API
- Type safe: `handleChange` properly infers the value of the field it is handling
- Context Provider
- Field component
- Validation strategy: `OnDemand` and `OnChange`
