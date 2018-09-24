module Helpers = ReForm_Helpers;
module Validation = ReForm_Validation;
module Value = ReForm_Value;
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
      | TrySubmit
      | HandleSubmitting(bool)
      | SetFieldsErrors(list((Config.fields, option(string))))
      | HandleFieldValidation((Config.fields, value))
      | HandleError(option(string))
      | HandleChange((Config.fields, value))
      | HandleSubmit
      | ResetFormState;
    type values = Config.state;
    type schema = list((Config.fields, Validation.validation(values)));
    module Field: {
      let getFieldLens:
        Config.fields =>
        (
          Config.fields,
          Config.state => value,
          (Config.state, value) => Config.state,
        );
      let validateField:
        (Config.fields, values, value, schema, Validation.I18n.dictionary) =>
        option(string);
      let handleChange: ((Config.fields, value), values) => values;
    };
    type onSubmit = {
      values,
      setSubmitting: bool => unit,
      setError: option(string) => unit,
      resetFormState: unit => unit,
    };
    type state = {
      values,
      isSubmitting: bool,
      errors: list((Config.fields, option(string))),
      error: option(string),
    };
    type reform = {
      form: state,
      handleChange: (Config.fields, value) => unit,
      handleGlobalValidation: option(string) => unit,
      handleSubmit: unit => unit,
      getErrorForField: Config.fields => option(string),
    };
    let component:
      ReasonReact.componentSpec(
        state,
        ReasonReact.stateless,
        ReasonReact.noRetainedProps,
        ReasonReact.noRetainedProps,
        action,
      );
    let make:
      (
        ~onSubmit: onSubmit => unit,
        ~onSubmitFail: list((Config.fields, option(string))) => unit=?,
        ~onFormStateChange: state => unit=?,
        ~validate: values => option(string)=?,
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
        action,
      );
  };
