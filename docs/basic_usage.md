---
id: basicUsage
title: Basic usage
---

A basic usage of ReForm looks like this:

First, create a param module that will be passed to the functor:

```reason
module SignUpFormParams = {
  /* Define the form state */
  type state = {
    password: string,
    confirmP: string,
    email: string
  };

  /* Defined the field types, used for validation and change handling */
  type fields = [ | `password | `email | `confirmPassword];

  /* Now teach ReForm how to get and set your fields given the types */
  /* The syntax goes (field, getter, setter) */
  let lens = [
    (`email, (s) => s.email, (s, email) => { ...s, email }),
    (`password, (s) => s.password, (s, password) => { ...s, password }),
    (`confirmPassword, (s) => s.confirmPassword, (s, confirmPassword) => { ...s, confirmPassword }),
  ];
};
```

Now you can generate the actual form container component:
```reason
module SignUpFormContainer = ReForm.Create(SignUpFormParams);
```

After creating the container module, you can use it as a React component:

```reason
module SignUpFormContainer = ReForm.Create(SignUpFormParams);

let defaults = (defaultValue, optional) => switch(optional) {
  | Some(value) => value
  | None => defaultValue
};

let component = ReasonReact.statelessComponent("SignUp");

let make = (_children) => {
  ...component,
  render: _self => {
    <SignUpFormContainer
      onSubmit=((~values, ~setError, ~setSubmitting) => doWhateverYouWant(~values, ~setError, ~setSubmitting))
      schema=[
        (`email, Email),
        (`password, Required),
        (`confirmPassword, Custom(s => s.password == s.confirmPassword ? None : Some("Passwords don't match")))
      ]
    >
    ...(({form, handleChange, handleSubmit, getErrorForField}) =>
      /* handleDomFormSubmit helps dealing with the DOM event for Web */
      <form onSubmit=ReForm.Helpers.handleDomFormSubmit(handleSubmit)>
       <label>
         <span> ("Email:" |> ReasonReact.stringToElement) </span>
         <input
           value=form.values.email
           onChange=(
             ReForm.Helpers.handleDomFormChange(handleChange(`email))
           )
         />
         <p>
           (
             getErrorForField(`email)
             |> defaults("")
             |> ReasonReact.stringToElement
           )
         </p>
       </label>
       <label>
         <span> ("Password:" |> ReasonReact.stringToElement) </span>
         <input
           _type="password"
           value=form.values.password
           onChange=(
             ReForm.Helpers.handleDomFormChange(
               handleChange(`password)
             )
           )
         />
         <p>
           (
             getErrorForField(`password)
             |> defaults("")
             |> ReasonReact.stringToElement
           )
         </p>
       </label>
       <label>
         <span>
           ("Confirm password:" |> ReasonReact.stringToElement)
         </span>
         <input
           _type="password"
           value=form.values.confirmPassword
           onChange=(
             ReForm.Helpers.handleDomFormChange(
               handleChange(`confirmPassword)
             )
           )
         />
         <p>
           (
             getErrorForField(`confirmPassword)
             |> defaults("")
             |> ReasonReact.stringToElement
           )
         </p>
       </label>
       <button _type="submit">
         ("Submit" |> ReasonReact.stringToElement)
       </button>
      </form>
    )
    </SignUpFormContainer>
  }
}
```
