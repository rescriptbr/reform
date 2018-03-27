module Helpers = ReForm_Helpers;

module Validation = ReForm_Validation;

module Value = ReForm_Value;

type validationErrors = ReForm_Validation.errors;

module type Config = {
  type state;
  type fields;
  let lens: list((fields, state => Value.t, (state, Value.t) => state));
};

module Create:
  (Config: Config) =>
  {
    type value = Value.t;
    type action =
      | HandleSubmitting(bool)
      | SetFieldsErrors(list((Config.fields, validationErrors)))
      | HandleFieldValidation((Config.fields, value))
      | HandleError(validationErrors)
      | HandleChange((Config.fields, value))
      | HandleSubmit
      | ResetFormState;
    type values = Config.state;
    type schema = list((Config.fields, list(Validation.validation(values))));
    module Field: {
      let getFieldLens:
        Config.fields =>
        (
          Config.fields,
          Config.state => value,
          (Config.state, value) => Config.state
        );
      let validateField:
        (Config.fields, values, value, schema, Validation.I18n.dictionary) =>
        validationErrors;
      let handleChange: ((Config.fields, value), values) => values;
    };
    type onSubmit = {
      values,
      setSubmitting: bool => unit,
      setError: validationErrors => unit,
      resetFormState: unit => unit
    };
    type state = {
      values,
      isSubmitting: bool,
      errors: list((Config.fields, validationErrors)),
      error: validationErrors
    };
    type reform = {
      form: state,
      handleChange: (Config.fields, value) => unit,
      handleGlobalValidation: validationErrors => unit,
      handleSubmit: unit => unit,
      getErrorForField: Config.fields => validationErrors
    };
    let component:
      ReasonReact.componentSpec(
        state,
        ReasonReact.stateless,
        ReasonReact.noRetainedProps,
        ReasonReact.noRetainedProps,
        action
      );
    let make:
      (
        ~onSubmit: onSubmit => unit,
        ~onFormStateChange: state => unit=?,
        ~validate: values => validationErrors=?,
        ~initialState: Config.state,
        ~schema: schema,
        ~i18n: Validation.I18n.dictionary=?,
        reform => ReasonReact.reactElement
      ) =>
      ReasonReact.componentSpec(
        state,
        state,
        ReasonReact.noRetainedProps,
        ReasonReact.noRetainedProps,
        action
      );
  };