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

open BsReform;

module Form = ReForm.Make(StateLenses);

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
            regExp(~matches="^[a-zA-Z\s]*$", Name)
            + custom(
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
            + float(~min=1.0, OpacityOfColors)
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
        onChange={BsReform.Helpers.handleChange(handleChange(Name))}
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
               onChange={BsReform.Helpers.handleChange(name =>
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
               onChange={Helpers.handleChange(hex =>
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
        onChange={Helpers.handleChange(handleChange(NumberOfFavoriteColors))}
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
        onChange={Helpers.handleChange(handleChange(OpacityOfColors))}
      />
      <p>
        {getFieldError(Field(OpacityOfColors))
         ->Belt.Option.getWithDefault("")
         ->ReasonReact.string}
      </p>
    </label>
  </form>;
};
