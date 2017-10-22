defmodule EjdbTest do
  use ExUnit.Case
  use Bitwise

  test "version" do
    assert Ejdb.version() === "1.2.12"
  end

  test "open db" do
    database_filename = "tmp/testdb"
    File.rm(database_filename)
    Ejdb.open(database_filename, bor(Ejdb.jbocreat(), Ejdb.jbowriter()))
    assert(File.exists?(database_filename), "Database not created")

    # Make sure free_db_resource runs
    :erlang.garbage_collect
  end
end
