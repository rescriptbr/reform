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

### NoValidation
Use for optional fields. NoValidation accepts any or no value
```reason
let schema = [
  (`name, NoValidation)
];

<SignUpFormParams schema>
```

### StringNonEmpty
Use for string that must have some value.
```reason
let schema = [
  (`name, StringNonEmpty)
];

<SignUpFormParams schema>
```

### StringRegExp
Use for when you need to build your own custom RegEx validation.
```reason
let schema = [
  (`name, StringRegExp("^[a-zA-Z\s]*$"))
];

<SignUpFormParams schema>
```

### StringMin
Use for string that must have minimum number of characters.
```reason
let schema = [
  (`name, StringMin(6))
];

<SignUpFormParams schema>
```

### StringMax
Use for string that must have maximum number of characters.
```reason
let schema = [
  (`name, StringMax(20))
];

<SignUpFormParams schema>
```

### IntMin
Use for integer that must have minimum value.
```reason
let schema = [
  (`age, IntMin(18))
];

<SignUpFormParams schema>
```

### IntMax
Use for integer that must have maximum value.
```reason
let schema = [
  (`age, IntMax(65))
];

<SignUpFormParams schema>
```

### FloatMin
Use for float that must have minimum value.
```reason
let schema = [
  (`price, FloatMin(5.0))
];

<SignUpFormParams schema>
```

### FloatMax
Use for float that must have maximum value.
```reason
let schema = [
  (`price, FloatMax(100.0))
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
