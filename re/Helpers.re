let handleDomFormChange = (handleChange, event) =>
  handleChange(
    ReactDOMRe.domElementToObj(ReactEventRe.Form.target(event))##value,
  );

let handleDomFormSubmit = (handleSubmit, event) => {
  ReactEventRe.Synthetic.preventDefault(event);
  handleSubmit();
};
