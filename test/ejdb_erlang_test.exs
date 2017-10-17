defmodule EjdbErlangTest do
  use ExUnit.Case
  doctest EjdbErlang

  test "greets the world" do
    assert EjdbErlang.hello() == :world
  end
end
