---
id: schema
title: Schemas
---

ReForm uses ReSchema to build schemas for validation, these are the available validators.
Validators are type safe and only accept the field of the type they are meant to operate with.

### Email(Lenses.field(string))
Validates email values
```reason
let schema = ReSchema.Validation([|
  Email(Lenses.Email),
|]);
```

### StringNonEmpty(Lenses.field(string))
Requires that the string within is not empty
```reason
let schema = ReSchema.Validation([|
  StringNonEmpty(Lenses.Name),
|]);
```

### NoValidation(Lenses.field('a))
Does not apply any validation in the field
```reason
let schema = ReSchema.Validation([|
  NoValidation(Lenses.Age),
|]);
```

### StringRegExp(Lenses.field(string), string)
Set a custom RegExp to match the value
```reason
let schema = ReSchema.Validation([|
  StringRegExp(Lenses.Name, "^[a-zA-Z\s]*$"),
|]);
```

### StringMin(Lenses.field(string), int)
Set a minimum length for the string
```reason
let schema = ReSchema.Validation([|
  StringMin(Lenses.Name, 3),
|]);
```

### StringMax(Lenses.field(string), int)
Set a maximum length for the string
```reason
let schema = ReSchema.Validation([|
  StringMax(Lenses.Name, 20),
|]);
```

### IntMin(Lenses.field(int), int)
Set a minumum value for the int
```reason
let schema = ReSchema.Validation([|
  IntMin(Lenses.Age, 21),
|]);
```

### IntMax(Lenses.field(int), int)
Set a maximum value for the int
```reason
let schema = ReSchema.Validation([|
  IntMax(Leses.Age, 60),
|]);
```

### FloatMin(Lenses.field(float), float)
Set a minimum value for the float
```reason
let schema = ReSchema.Validation([|
  FloatMin(Lenses.Price, 1.0),
|]);
```

### FloatMax(Lenses.field(float), float)
Set a maximum value for the float
```reason
let schema = ReSchema.Validation([|
  FloatMax(Lenses.Price, 100.0),
|]);
```

### Custom(Lenses.field('a), Lenses.state => fieldState)
Useful for when you need to build your own custom logic. 
You can validate with props your component received - this is why we let you pass the schema as prop and not in the functor - or whatever you need.
