defmodule EjdbTest do
  use ExUnit.Case
  doctest Ejdb

  test "version" do
    assert Ejdb.version() === "1.2.12"
  end
end
