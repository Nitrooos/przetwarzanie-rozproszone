package role.client.command;

import data.Database;

public class Quit implements Command {

    private String _additionalName = "";

    public Quit() { }

    public Quit(String additionalName) {
        this._additionalName = additionalName;
    }

    @Override
    public boolean run(Database db) {
        return false;
    }

    @Override
    public String name() {
        return "Wyjście " + this._additionalName;
    }
}
