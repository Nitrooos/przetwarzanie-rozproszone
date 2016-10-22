package data;

import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.util.Map;
import java.util.TreeMap;

public class PeopleDatabase extends UnicastRemoteObject implements Database {
    private Map<Integer, Person> _people = new TreeMap();
    private Integer _next_id = 0;

    public PeopleDatabase() throws RemoteException {
        super();
    }

    @Override
    synchronized public Person create(Person p) {
        Integer id = this._next_id;
        this._people.put(id, p);
        this._next_id++;
        return this._people.get(id);
    }

    @Override
    synchronized public Map<Integer, Person> getAll() {
        return this._people;
    }

    @Override
    synchronized public Person get(int id) {
        return this._people.get(id);
    }

    @Override
    synchronized public Map<Integer, Person> search(String query) {
        Map<Integer, Person> found_people = new TreeMap();
        for (Map.Entry<Integer, Person> person: this._people.entrySet()) {
            if (person.getValue().getInfo().match(query)) {
                found_people.put(person.getKey(), person.getValue());
            }
        }
        return found_people;
    }

    @Override
    synchronized public Integer findId(Person person) throws RemoteException {
        for (Map.Entry<Integer, Person> entry : this._people.entrySet()) {
            Person p = entry.getValue();
            if (p.getFullName().equals(person.getFullName()) &&
                p.getInfo().getBirth().equals(person.getInfo().getBirth()) &&
                p.getInfo().getGender().equals(person.getInfo().getGender()))
            {
                return entry.getKey();
            }
        }
        return -1;
    }

    @Override
    synchronized public Person update(int id, Person update) {
        Person p = this._people.get(id);
        if (p != null) {
            p.update(update);
            return p;
        }

        return null;
    }

    @Override
    public Person delete(int id) {
        return this._people.remove(id);
    }
}
