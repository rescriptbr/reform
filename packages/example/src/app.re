[%bs.raw {|require('./app.css')|}];

[@bs.module] external logo: string = "./logo.svg";

let component = ReasonReact.statelessComponent("App");

let make = (~message, _children) => {
  ...component,
  render: _self =>
    <ReasonApollo.Provider client=Apollo.client>
      <div className="App">
        <div className="App-header">
          <img
            src="https://astrocoders.com/images/hero/logo.png"
            className="App-logo"
            alt="logo"
          />
          <h2> {ReasonReact.string(message)} </h2>
        </div>
        <p className="App-intro">
          {ReasonReact.string("Say hello to ReForm")}
        </p>
        <PostAdd />
        <h1>{ReasonReact.string("ReFormNext demo")}</h1>
        <PostAddNext/>
      </div>
    </ReasonApollo.Provider>,
};
