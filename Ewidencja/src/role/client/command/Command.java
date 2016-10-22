package role.client.command;

import data.Database;

public interface Command {
    boolean run(Database db);
    String name();
}
