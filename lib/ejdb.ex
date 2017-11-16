defmodule Ejdb do
  @moduledoc """
  EJDB embeddable database with MongoDB like features
  """

  @on_load { :init, 0 }

  app = Mix.Project.config[:app]

  def init do
    path = :filename.join(:code.priv_dir(unquote(app)), 'ejdb')
    :ok = :erlang.load_nif(path, 0)
  end

  

  @doc "Ejdblib version"
  def version, do: missing_nif()

  @doc """
  Open database with name `name`
  
  The opening `mode` is given as a list of atoms:
  - :jboreader Open as a reader
  - :jbowriter Open as a writer
  - :jbocreat Create if db file not exists
  - :jbotrunc Truncate db on open.
  - :jbonolck Open without locking
  - :jbolcknb Lock without blocking
  - :jbotsync Synchronize every transaction
  """
  @spec open(binary, list(atom)) :: {:ok, reference} | {:error, binary}
  def open(filename, mode)
  def open(_, _), do: missing_nif()

  @doc "Create collection"
  def create_collection(db, name, options \\ [])
  def create_collection(_, _, _), do: missing_nif()

  @doc "Save a marshalled bson into a collection"
  def save_bson(collection, bson)
  def save_bson(_, _), do: missing_nif()

  defp missing_nif, do: raise "Missing NIF"
end
