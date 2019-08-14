let handleDomFormChange = (handleChange, event) =>
  handleChange(ReactEvent.Form.target(event)##value);
let handleDomFormSubmit = (handleSubmit, event) => {
  ReactEvent.Synthetic.preventDefault(event);
  handleSubmit();
};
