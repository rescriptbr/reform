[%bs.raw {|require('./app.css')|}];

[@bs.module] external logo : string = "./logo.svg";

let component = ReasonReact.statelessComponent("App");

let make = (~message, _children) => {
  ...component,
  render: _self =>
    <div className="App">
      <div className="App-header">
        <img
          src="https://astrocoders.com/images/hero/logo.png"
          className="App-logo"
          alt="logo"
        />
        <h2> (ReasonReact.stringToElement(message)) </h2>
      </div>
      <p className="App-intro">
        (ReasonReact.stringToElement("Say hello to ReForm"))
      </p>
      <PostAdd />
    </div>,
};
