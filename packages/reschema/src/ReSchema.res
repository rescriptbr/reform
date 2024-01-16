module type Lenses = {
  type field<'a>
  type state
  let set: (state, field<'a>, 'a) => state
  let get: (state, field<'a>) => 'a
}

type childFieldError = {
  error: string,
  index: int,
  name: string,
}

type fieldState =
  | Valid
  | NestedErrors(array<childFieldError>)
  | Error(string)

type recordValidationState<'a> =
  | Valid
  | Errors(array<('a, string)>)

module Make = (Lenses: Lenses) => {
  type rec field = Field(Lenses.field<'a>): field

  module Validation = {
    type rec t =
      | Email({field: Lenses.field<string>, error: option<string>}): t
      | NoValidation({field: Lenses.field<'a>}): t
      | StringNonEmpty({field: Lenses.field<string>, error: option<string>}): t
      | StringRegExp({field: Lenses.field<string>, matches: string, error: option<string>}): t
      | StringMin({field: Lenses.field<string>, min: int, error: option<string>}): t
      | StringMax({field: Lenses.field<string>, max: int, error: option<string>}): t
      | IntMin({field: Lenses.field<int>, min: int, error: option<string>}): t
      | IntMax({field: Lenses.field<int>, max: int, error: option<string>}): t
      | FloatMin({field: Lenses.field<float>, min: float, error: option<string>}): t
      | FloatMax({field: Lenses.field<float>, max: float, error: option<string>}): t
      | Custom({field: Lenses.field<'a>, predicate: Lenses.state => fieldState}): t
      | True({field: Lenses.field<bool>, error: option<string>}): t
      | False({field: Lenses.field<bool>, error: option<string>}): t

    type schema = array<t>

    let schema = validations => validations->Belt.Array.concatMany

    let mergeValidators = validations => {
      validations->Belt.Array.reduce([], (rules, (rule, apply)) =>
        switch rule {
        | None => rules
        | Some(rule) => rules->Belt.Array.concat([rule->apply])
        }
      )
    }

    let custom = (predicate, field) => [Custom({field: field, predicate: predicate})]

    let true_ = (~error=?, field) => [True({field: field, error: error})]

    let false_ = (~error=?, field) => [False({field: field, error: error})]

    let email = (~error=?, field) => [Email({field: field, error: error})]

    let nonEmpty = (~error=?, field) => [StringNonEmpty({field: field, error: error})]

    let string = (~min=?, ~minError=?, ~max=?, ~maxError=?, field) => {
      mergeValidators([
        (
          min,
          min => StringMin({
            field: field,
            min: min,
            error: minError,
          }),
        ),
        (
          max,
          max => StringMax({
            field: field,
            max: max,
            error: maxError,
          }),
        ),
      ])
    }

    let regExp = (~error=?, ~matches, field) => [
      StringRegExp({field: field, matches: matches, error: error}),
    ]

    let float = (~min=?, ~minError=?, ~max=?, ~maxError=?, field) => {
      mergeValidators([
        (
          min,
          min => FloatMin({
            field: field,
            min: min,
            error: minError,
          }),
        ),
        (
          max,
          max => FloatMax({
            field: field,
            max: max,
            error: maxError,
          }),
        ),
      ])
    }

    let int = (~min=?, ~minError=?, ~max=?, ~maxError=?, field) => {
      mergeValidators([
        (
          min,
          min => IntMin({
            field: field,
            min: min,
            error: minError,
          }),
        ),
        (
          max,
          max => IntMax({
            field: field,
            max: max,
            error: maxError,
          }),
        ),
      ])
    }
  }

  let validateField = (~validator, ~values, ~i18n: ReSchemaI18n.t): (field, fieldState) =>
    switch validator {
    | Validation.True({field, error}) =>
      let value = Lenses.get(values, field)
      (Field(field), value ? Valid : Error(error->Belt.Option.getWithDefault(i18n.true_())))
    | Validation.False({field, error}) =>
      let value = Lenses.get(values, field)
      (
        Field(field),
        value == false ? Valid : Error(error->Belt.Option.getWithDefault(i18n.false_())),
      )
    | Validation.IntMin({field, min, error}) =>
      let value = Lenses.get(values, field)
      (
        Field(field),
        value >= min ? Valid : Error(error->Belt.Option.getWithDefault(i18n.intMin(~value, ~min))),
      )
    | Validation.IntMax({field, max, error}) =>
      let value = Lenses.get(values, field)

      (
        Field(field),
        value <= max ? Valid : Error(error->Belt.Option.getWithDefault(i18n.intMax(~value, ~max))),
      )
    | Validation.FloatMin({field, min, error}) =>
      let value = Lenses.get(values, field)
      (
        Field(field),
        value >= min
          ? Valid
          : Error(error->Belt.Option.getWithDefault(i18n.floatMin(~value, ~min))),
      )
    | Validation.FloatMax({field, max, error}) =>
      let value = Lenses.get(values, field)
      (
        Field(field),
        Lenses.get(values, field) <= max
          ? Valid
          : Error(error->Belt.Option.getWithDefault(i18n.floatMax(~value, ~max))),
      )
    | Validation.Email({field, error}) =>
      let value = Lenses.get(values, field)
      (
        Field(field),
        Js.Re.test_(ReSchemaRegExp.email, value)
          ? Valid
          : Error(error->Belt.Option.getWithDefault(i18n.email(~value))),
      )
    | Validation.NoValidation({field}) => (Field(field), Valid)
    | Validation.StringNonEmpty({field, error}) =>
      let value = Lenses.get(values, field)
      (
        Field(field),
        value === ""
          ? Error(error->Belt.Option.getWithDefault(i18n.stringNonEmpty(~value)))
          : Valid,
      )
    | Validation.StringRegExp({field, matches, error}) =>
      let value = Lenses.get(values, field)
      (
        Field(field),
        Js.Re.test_(Js.Re.fromString(matches), value)
          ? Valid
          : Error(error->Belt.Option.getWithDefault(i18n.stringRegExp(~value, ~pattern=matches))),
      )
    | Validation.StringMin({field, min, error}) =>
      let value = Lenses.get(values, field)
      (
        Field(field),
        Js.String.length(value) >= min
          ? Valid
          : Error(error->Belt.Option.getWithDefault(i18n.stringMin(~value, ~min))),
      )
    | Validation.StringMax({field, max, error}) =>
      let value = Lenses.get(values, field)
      (
        Field(field),
        Js.String.length(value) <= max
          ? Valid
          : Error(error->Belt.Option.getWithDefault(i18n.stringMax(~value, ~max))),
      )
    | Validation.Custom({field, predicate}) => (Field(field), predicate(values))
    }

  let getFieldValidator = (~validators, ~fieldName) =>
    validators->Belt.Array.getBy(validator =>
      switch validator {
      | Validation.False({field}) => Field(field) == fieldName
      | Validation.True({field}) => Field(field) == fieldName
      | Validation.IntMin({field}) => Field(field) == fieldName
      | Validation.IntMax({field}) => Field(field) == fieldName
      | Validation.FloatMin({field}) => Field(field) == fieldName
      | Validation.FloatMax({field}) => Field(field) == fieldName
      | Validation.Email({field}) => Field(field) == fieldName
      | Validation.NoValidation({field}) => Field(field) == fieldName
      | Validation.StringNonEmpty({field}) => Field(field) == fieldName
      | Validation.StringRegExp({field}) => Field(field) == fieldName
      | Validation.StringMin({field}) => Field(field) == fieldName
      | Validation.StringMax({field}) => Field(field) == fieldName
      | Validation.Custom({field}) => Field(field) == fieldName
      }
    )

  let getFieldValidators = (~validators, ~fieldName) =>
    validators->Belt.Array.keep(validator =>
      switch validator {
      | Validation.False({field}) => Field(field) == fieldName
      | Validation.True({field}) => Field(field) == fieldName
      | Validation.IntMin({field}) => Field(field) == fieldName
      | Validation.IntMax({field}) => Field(field) == fieldName
      | Validation.FloatMin({field}) => Field(field) == fieldName
      | Validation.FloatMax({field}) => Field(field) == fieldName
      | Validation.Email({field}) => Field(field) == fieldName
      | Validation.NoValidation({field}) => Field(field) == fieldName
      | Validation.StringNonEmpty({field}) => Field(field) == fieldName
      | Validation.StringRegExp({field}) => Field(field) == fieldName
      | Validation.StringMin({field}) => Field(field) == fieldName
      | Validation.StringMax({field}) => Field(field) == fieldName
      | Validation.Custom({field}) => Field(field) == fieldName
      }
    )

  let validateOne = (
    ~field: field,
    ~values,
    ~i18n=ReSchemaI18n.default,
    schema: Validation.schema,
  ) => {
    getFieldValidators(~validators=schema, ~fieldName=field)
    ->Belt.Array.map(validator => validateField(~validator, ~values, ~i18n))
    ->Belt.Array.getBy(validation => {
      switch validation {
      | (_, Error(_)) => true
      | _ => false
      }
    })
  }

  let validateFields = (~fields, ~values, ~i18n, schema: Validation.schema) => {
    Belt.Array.map(fields, field =>
      getFieldValidator(~validators=schema, ~fieldName=field)->Belt.Option.map(validator =>
        validateField(~validator, ~values, ~i18n)
      )
    )
  }

  let validate = (~i18n=ReSchemaI18n.default, values: Lenses.state, schema: Validation.schema) => {
    let validationList =
      schema->Belt.Array.map(validator => validateField(~validator, ~values, ~i18n))

    let errors = validationList->Belt.Array.keepMap(((field, fieldState)) =>
      switch fieldState {
      | Error(string) => Some((field, string))
      | _ => None
      }
    )

    Belt.Array.length(errors) > 0 ? Errors(errors) : Valid
  }
}
