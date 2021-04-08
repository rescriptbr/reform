module Int = Js.Int

type spacing = (int, int, int, int)

let toRem = value => value->Int.toFloat *. 0.8 |> Css.rem

let margins = ((top, left, bottom, right)) =>
  CssJs.margin4(~top=top->toRem, ~left=left->toRem, ~bottom=bottom->toRem, ~right=right->toRem)

let paddings = ((top, left, bottom, right)) =>
  CssJs.padding4(~top=top->toRem, ~left=left->toRem, ~bottom=bottom->toRem, ~right=right->toRem)

let box = (
  ~direction as direction_,
  ~align,
  ~justify,
  ~block as block_,
  ~fullHeight,
  ~margin as marginT,
  ~padding as paddingT,
  ~maxWidth as maxWidthValue,
) => {
  open CssJs

  style(.[
    display(#flex),
    flexDirection(direction_),
    alignItems(align),
    justifyContent(justify),
    width(block_ ? 100.0->pct : auto),
    height(fullHeight ? 100.0->pct : auto),
    margins(marginT),
    paddings(paddingT),
    maxWidth(maxWidthValue),
  ])
}

@react.component
let make = (
  ~children,
  ~direction=#row,
  ~align=#center,
  ~justify=#center,
  ~block=true,
  ~fullHeight=false,
  ~margin=(0, 0, 0, 0),
  ~padding=(0, 0, 0, 0),
  ~maxWidth=CssJs.none,
) => {
  let className = box(
    ~direction,
    ~align,
    ~justify,
    ~block,
    ~margin,
    ~padding,
    ~fullHeight,
    ~maxWidth,
  )

  <div className> children </div>
}
