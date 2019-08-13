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

### StringNonEmpty(Lenses.field(string))
### StringRegExp(Lenses.field(string), string)
### StringMin(Lenses.field(string), int)
### StringMax(Lenses.field(string), int)
### IntMin(Lenses.field(int), int)
### IntMax(Lenses.field(int), int)
### FloatMin(Lenses.field(float), float)
### FloatMax(Lenses.field(float), float)

### Custom(Lenses.field('a), Lenses.state => fieldState)
Useful for when you need to build your own custom logic. 
You can validate with props your component received - this is why we let you pass the schema as prop and not in the functor - or whatever you need.
