---
title: Getting started
id: getting-started
slug: /getting-started
---

import CodeBlock from '../__components/CodeBlock.bs'
// Sources
import HandlingChangesSource from '!!raw-loader!./SignUpForm_HandlingChanges.res'
import MarkupSource from '!!raw-loader!./SignUpForm_Markup.res'
import FormHookSource from '!!raw-loader!./SignUpForm_FormHook.res'
import SubmitSource from '!!raw-loader!./SignUpForm_Submit.res'
// Preview
import { make as HandlingChangesPreview } from './SignUpForm_HandlingChanges.bs.js'
import { make as MarkupPreview } from './SignUpForm_Markup.bs.js'
import { make as FormHookPreview } from './SignUpForm_FormHook.bs.js'
import { make as SubmitPreview } from './SignUpForm_Submit.bs.js'
import { Preview } from '../__components'

After setting up `reform`, `reschema`, and `lenses-ppx` in the [previous section](/docs/installation), you're ready to create your first form. You can understand every
part of this tutorial by reading the **API reference**, but we know that you want a sneak peek before getting deeper into the API.

## Quick start 

### Creating our form
First off, we need to create our "lenses module" using lenses-ppx. Don't worry if you don't know what a ppx is, you can keep going just following our instructions.
We need to create a record that represents the state of the form. In this tutorial, the form has three fields: `name`, `email`, and `password`.

```rescript title="SignUpForm.res"
module FormFields = %lenses(
  type state = {
    name: string,
    email: string,
    password: string
  }
)
```
:::caution
The name of the record passed to `lenses-ppx` must be named as `state`.
:::

You might be asking yourself: _"what this lenses-ppx is doing?"_ and it's kind of magic, but it's a way to create "getters" and "setters" for the `state` record.

After that, we have to create a new form using the `ReForm.Make` module functor. 
The module functor expects a lenses module that was created by
`lenses-ppx` and returns a new form module. You can see the API reference of this module [here](/docs/reform-make).

```rescript {9} title="SignUpForm.res" 
module FormFields = %lenses(
  type state = {
    name: string,
    email: string,
    password: string,
  }
)

module Form = ReForm.Make(FormFields)
```
:::tip
You can read more about module functors [here](https://rescript-lang.org/docs/manual/v8.0.0/module#module-functions-functors).
:::

### The `Form.use` hook
ReForm provides a form hook and we're going to use it by passing some parameters like `schema`, an `onSubmit` function, `initialState`, etc.

<CodeBlock title="SignUpForm.res" language="rescript"> {FormHookSource}</CodeBlock>

We can split this snippet into four parts:
- 1. The `Form.use` hook calling: This is the hook provided by reform. It returns a `form` record that is typed as `Form.api` and you can read more about its api _here_.
- 2. The `onSubmit` parameter: Just a function that will be called when you trigger the `form.submit` function.
- 3. The `validationStrategy`: We're telling to reform which strategy of validation we want to use, in this case, we're using `OnDemand` which means that we'll trigger the validation **manually** using the `form` record.
- 4. The `schema` parameter: It's a schema created using [ReSchema](https://github.com/rescriptbr/reschema). You can read more about the usage of _reschema with reform_ and in its _official documentation_.

### Creating our form component
We need a form to make everything work, so we're going to use a combination of inputs and buttons to create a simple sign up form:
:::important
For this tutorial, we created some local components (like `Input`, `Button`, `Input.Error`) just to make the markup more readable, but with the same API (`onChange`, `value`, `onClick`, etc). Another components like `Box` or `Typography` are from [Ancestor](https://github.com/rescriptbr/reform) which is
an ui library and is totally optional for this tutorial. Feel free to use pure html with or without css to create your form.
:::
<CodeBlock title="SignUpForm.res" language="rescript"> {MarkupSource}</CodeBlock>

<Preview>
  <MarkupPreview/>
</Preview>


### Integrating the form

We created the `Form` module by combining `lenses-ppx`, `reform`, and `reschema` and we also have a simple form component. Now, it's time to make everything work together. 

Different from libraries like [react-hook-form](https://react-hook-form.com/), ReForm doesn't use any kind of magic with refs.
ReForm was created to be both deadly simple and to make forms sound good, leveraging ReScript's powerful typesytem. 
Even the schemas we use are nothing more than constructors built-in in the language itself.

We encourage you to handle every change in your inputs manually. Not just the changes, but also the conversion of values, like string to int or string to float.
Might be more verbose to do everything manually, but it's intentional and keep you in control of everything that happens with your forms.

The `form` record returned by reform has some fields like `handleChange` and `values` that we're going to use to integrate the form module with our form component.
We're going to start by handling the changes on the inputs:

<CodeBlock metastring="{45-46,52-53,60-61,65-71}" title="SignUpForm.res" language="rescript"> {HandlingChangesSource}</CodeBlock>

Also, we've added a simple block to display the form values. If you type something in any input, you can see the values changing:
<Preview>
  <HandlingChangesPreview />
</Preview>

<br />

But, we can just type and see the values, if you click on the submit button, nothing happens, no error messages, no console.log, etc.
To make everything work, we still have to do two things: trigger the `form.submit` function and render the validation errors (when we got an error) using the `form.getFieldError` function:

<br />

<CodeBlock metastring="{46,57,69,79-80}" title="SignUpForm.res" language="rescript"> {SubmitSource}</CodeBlock>

Now, when we click on the submit button without filling the form (or filling with invalid values), we can see the error message for each field.
Also, if we open the browser console and fill all fields correctly, we can see the result of the form submission on the console.

<Preview>
  <SubmitPreview />
</Preview>

<br/>


### Disclaimers

There are some disclaimers about this part of the tutorial:

- The first one is about the handling of our `handleSubmit` function. If you click on the submit button without filling the fields, the function
will not be triggered. That's the expected behavior. The `onSubmit` function will be triggered when the form is valid. If you need to handle
a function on the submit and there are invalid fields, you can use the `onSubmitFail` parameter. You can read more about this **here**.
- Because we're passing `OnDemand` to the `validationStrategy` parameter, we have to call the form validation **manually** using a function like `form.validateForm` or just
call the `form.submit` function (like we did) that triggers the form validation automatically. If you need to trigger the validation on every change, you can use `OnChange` as a validation strategy.
You can read more about validation strategies **here**.
