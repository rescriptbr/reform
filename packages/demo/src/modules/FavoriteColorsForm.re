module FavoriteColorsLenses = [%lenses
  type t = {
    name: string,
    hex: string,
  }
];

module StateLenses = [%lenses
  type state = {
    name: string,
    favoriteColors: array(FavoriteColorsLenses.t),
    numberOfFavoriteColors: int,
    opacityOfColors: float,
  }
];

module Form = ReForm.Make(StateLenses);

type meta =
  | Mask(string)
  | Slider;

[@react.component]
let make = () => {
  let {
    state,
    submit,
    getFieldError,
    handleChange,
    arrayPush,
    arrayUpdateByIndex,
    arrayRemoveByIndex,
  }: Form.api =
    Form.use(
      ~schema={
        Form.Validation.(
          Schema(
            regExp(~matches="^[a-zA-Z\s]*$", ~meta=Mask("000"), Name)
            + custom(
                ~meta=Mask("dkljfas"),
                ({favoriteColors}) => {
                  let length = Array.length(favoriteColors);

                  length < 0
                  || Belt.Array.some(favoriteColors, favColor =>
                       Js.String.length(favColor.hex) == 0
                     )
                    ? Error("Invalid colors") : Valid;
                },
                FavoriteColors,
              )
            + float(~min=1.0, ~meta=Slider, OpacityOfColors)
            + int(~max=3, NumberOfFavoriteColors),
          )
        );
      },
      ~onSubmit=({state}) => None,
      ~initialState={
        name: "",
        favoriteColors: [||],
        numberOfFavoriteColors: 0,
        opacityOfColors: 0.0,
      },
      (),
    );

  <form
    onSubmit={event => {
      ReactEvent.Synthetic.preventDefault(event);
      submit();
    }}>
    <label>
      <span> {"Name:" |> ReasonReact.string} </span>
      <input
        value={state.values.name}
        onChange={ReForm.Helpers.handleChange(handleChange(Name))}
      />
      <p>
        {getFieldError(Field(Name))
         ->Belt.Option.getWithDefault("")
         ->ReasonReact.string}
      </p>
    </label>
    <button onClick={_ => arrayPush(FavoriteColors, {name: "", hex: ""})}>
      {React.string("Add color")}
    </button>
    {state.values.favoriteColors
     ->Belt.Array.mapWithIndex((index, favColor) =>
         <>
           <hr />
           <label>
             <span> {"Name:" |> ReasonReact.string} </span>
             <input
               value={favColor.name}
               onChange={ReForm.Helpers.handleChange(name =>
                 arrayUpdateByIndex(
                   ~field=FavoriteColors,
                   ~index,
                   {...favColor, name},
                 )
               )}
             />
           </label>
           <label>
             <span> {"Hex:" |> ReasonReact.string} </span>
             <input
               value={favColor.hex}
               onChange={ReForm.Helpers.handleChange(hex =>
                 arrayUpdateByIndex(
                   ~field=FavoriteColors,
                   ~index,
                   {...favColor, hex},
                 )
               )}
             />
           </label>
           <button onClick={_ => arrayRemoveByIndex(FavoriteColors, index)}>
             {React.string("Remove")}
           </button>
           <hr />
         </>
       )
     ->React.array}
    <label>
      <span> {"Number of favorite colors:" |> ReasonReact.string} </span>
      <input
        value={string_of_int(state.values.numberOfFavoriteColors)}
        type_="number"
        onChange={ReForm.Helpers.handleChange(
          handleChange(NumberOfFavoriteColors),
        )}
      />
      <p>
        {getFieldError(Field(NumberOfFavoriteColors))
         ->Belt.Option.getWithDefault("")
         ->ReasonReact.string}
      </p>
    </label>
    <label>
      <span> {"Opacity of colors:" |> ReasonReact.string} </span>
      <input
        value={Js.Float.toString(state.values.opacityOfColors)}
        type_="number"
        step=0.1
        onChange={ReForm.Helpers.handleChange(handleChange(OpacityOfColors))}
      />
      <p>
        {getFieldError(Field(OpacityOfColors))
         ->Belt.Option.getWithDefault("")
         ->ReasonReact.string}
      </p>
    </label>
  </form>;
};
