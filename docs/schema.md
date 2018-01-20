---
id: schema
title: Schemas
---

ReForm comes with helpful validation schema validators, here are the available ones:

### Email
Use for email values
```reason
let schema = [
  (`email, Email),
];

<SignUpFormParams schema>
```

### Required
For required changes. For now Required checks if the value is `""` to throw the error.
```reason
let schema = [
  (`name, Required)
];

<SignUpFormParams schema>
```

### Custom(Config.state => option(string))
Useful for when you need to build your own custom logic. 
You can validate with props your component received - this is why we let you pass the schema as prop and not in the functor - or whatever you need.

```reason
let make = (~isAdmin, _children) => {
  ...component,
  render: _self =>
    let schema = [
      (`password, Custom(
        s => s.password === "12345" && isAdmin ?
        Some("As an admin you need to have a stronger password") : None
      ))
    ]

    <SignUpFormParams schema>
}
```
