(* Representation of type application.  This corresponds to the "apply"
   variant type in a defunctionalized program.  Application is postfix. *)
type ('p, 'f) app

module type Newtype0 = sig
  type s
  type t
  external inj : s -> t 
    = "%identity"
  external prj : t -> s
    = "%identity"
end

module type Newtype1 = sig
  type 'a s
  type t
  external inj : 'a s -> ('a, t) app 
    = "%identity"
  external prj : ('a, t) app -> 'a s
    = "%identity"
end

module type Newtype2 = sig
  type ('a, 'b) s
  type t
  external inj : ('a, 'b) s -> ('a, ('b, t) app) app 
    = "%identity"
  external prj : ('a, ('b, t) app) app -> ('a, 'b) s
    = "%identity"
end

module type Newtype3 = sig
  type ('a, 'b, 'c) s
  type t
  external inj : ('a, 'b, 'c) s -> ('a, ('b, ('c, t) app) app) app 
    = "%identity"
  external prj : ('a, ('b, ('c, t) app) app) app -> ('a, 'b, 'c) s
    = "%identity"
end

module type Newtype4 = sig
  type ('a, 'b, 'c, 'd) s
  type t
  external inj : ('a, 'b, 'c, 'd) s -> ('a, ('b, ('c, ('d, t) app) app) app) app 
    = "%identity"
  external prj : ('a, ('b, ('c, ('d, t) app) app) app) app -> ('a, 'b, 'c, 'd) s
    = "%identity"
end

module type Newtype5 = sig
  type ('a, 'b, 'c, 'd, 'e) s
  type t
  external inj : ('a, 'b, 'c, 'd, 'e) s -> ('a, ('b, ('c, ('d, ('e, t) app) app) app) app) app 
    = "%identity"
  external prj : ('a, ('b, ('c, ('d, ('e, t) app) app) app) app) app -> ('a, 'b, 'c, 'd, 'e) s
    = "%identity"
end

module type Newtype6 = sig
  type ('a, 'b, 'c, 'd, 'e, 'f) s
  type t
  external inj : ('a, 'b, 'c, 'd, 'e, 'f) s -> ('a, ('b, ('c, ('d, ('e, ('f, t) app) app) app) app) app) app 
    = "%identity"
  external prj : ('a, ('b, ('c, ('d, ('e, ('f, t) app) app) app) app) app) app -> ('a, 'b, 'c, 'd, 'e, 'f) s
    = "%identity"
end

module Common =
struct
  type t
  external inj : 'a -> 'b = "%identity"
  external prj : 'a -> 'b = "%identity"
end

module Newtype0 (T : sig type t end) =
struct
  type s = T.t
  include Common
end

module Newtype1 (T : sig type 'a t end) =
struct
  type 'a s = 'a T.t
  include Common
end

module Newtype2 (T : sig type ('a, 'b) t end) =
struct
  type ('a, 'b) s = ('a, 'b) T.t
  include Common
end

module Newtype3 (T : sig type ('a, 'b, 'c) t end) =
struct
  type ('a, 'b, 'c) s = ('a, 'b, 'c) T.t
  include Common
end

module Newtype4 (T : sig type ('a, 'b, 'c, 'd) t end) =
struct
  type ('a, 'b, 'c, 'd) s = ('a, 'b, 'c, 'd) T.t
  include Common
end

module Newtype5 (T : sig type ('a, 'b, 'c, 'd, 'e) t end) =
struct
  type ('a, 'b, 'c, 'd, 'e) s = ('a, 'b, 'c, 'd, 'e) T.t
  include Common
end

module Newtype6 (T : sig type ('a, 'b, 'c, 'd, 'e, 'f) t end) =
struct
  type ('a, 'b, 'c, 'd, 'e, 'f) s = ('a, 'b, 'c, 'd, 'e, 'f) T.t
  include Common
end
