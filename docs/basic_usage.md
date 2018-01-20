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
    confirm: string,
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
```
module SignUpFormContainer = ReForm.Create(SignUpFormParams);
```
