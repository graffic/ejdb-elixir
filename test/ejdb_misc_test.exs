defmodule EjdbTest.Misc do
    use ExUnit.Case

  test "version" do
    assert Ejdb.version() === "1.2.12"
  end
end