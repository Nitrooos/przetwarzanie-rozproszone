package data;

import java.io.Serializable;
import java.text.SimpleDateFormat;
import java.util.*;

public class Person implements Serializable {
    public class MotherMustBeFemale extends RuntimeException {}
    public class FatherMustBeMale   extends RuntimeException {}

    private BaseInfo _info;
    private Person _mother;
    private Person _father;
    private List<Person> _children = new ArrayList();
    private List<Marriage> _marriages = new ArrayList();

    public Person(BaseInfo info) {
        this._info = info;
    }

    public void update(Person p) {
        this._info = p._info;
        this._mother = p._mother;
        this._father = p._father;
        this._marriages = p._marriages;
        this._children = p._children;
    }

    public BaseInfo getInfo() {
        return this._info;
    }

    public Person mother(Person mother) {
        if (mother.getInfo().getGender() != BaseInfo.Gender.FEMALE) {
            throw new MotherMustBeFemale();
        }

        this._mother = mother;
        return this;
    }

    public Person father(Person father) {
        if (father.getInfo().getGender() != BaseInfo.Gender.MALE) {
            throw new FatherMustBeMale();
        }

        this._father = father;
        return this;
    }

    public Person getMother() {
        return this._mother;
    }

    public Person getFather() {
        return this._father;
    }

    public List<Person> getChildren() {
        return this._children;
    }

    public String getFullName() { return this._info.getFirstname() + " " + this._info.getLastname(); }

    public void addMarriage(Marriage m) {
        this._marriages.add(m);
    }

    public boolean isMarried() {
        return this._marriages.stream().anyMatch(m -> m.getEnd() == null);
    }

    public Optional<Marriage> getCurrentMarriage() {
        if (this._marriages != null) {
            return this._marriages.stream().filter(m -> m.getEnd() == null).findFirst();
        }
        return Optional.empty();
    }

    public void addChild(Person p) {
        this._children.add(p);
    }

    public void printMarriagesInfo() {
        System.out.println("Małżeństwa: ");
        if (this._marriages.size() == 0) {
            System.out.println("brak");
            return;
        }

        Collections.sort(this._marriages, (m1, m2) -> {
            if (m1.getStart().after(m2.getStart())) {
                return 1;
            } else {
                return -1;
            }
        });

        for (Marriage marriage: this._marriages) {
            System.out.println("   " + marriage.getSpouse(this).getFullName() + marriage.getPeriod());
        }
    }

    public void printChildrenInfo() {
        System.out.println("Dzieci: ");
        if (this._children.size() == 0) {
            System.out.println("brak");
            return;
        }

        for (Person child : this._children) {
            System.out.println("   " + child.getFullName());
        }
    }

    public void printParentInfo() {
        String mother = this._mother != null ? this._mother.getFullName() : "brak",
               father = this._father != null ? this._father.getFullName() : "brak";
        System.out.println("Rodzice: " + mother + ", " + father);
    }

    static public void printTable(Map<Integer, Person> people) {
        if (people.size() == 0) {
            System.out.println("Brak rekordów w bazie!\n");
        } else {
            System.out.format("%-5s%-15s%-15s%-15s%-10s", "ID", "| Imię", "| Nazwisko", "| Data ur1.", "| Płeć");
            System.out.println("");
            System.out.println("----------------------------------------------------------");
            for (Map.Entry<Integer, Person> entry : people.entrySet()) {
                BaseInfo info = entry.getValue().getInfo();
                System.out.format("%-5s%-15s%-15s%-15s%-10s",
                        entry.getKey(),
                        "| " + info.getFirstname(),
                        "| " + info.getLastname(),
                        "| " + new SimpleDateFormat("dd.MM.yyyy").format(info.getBirth()),
                        "| " + info.getGender());
                System.out.println("");
            }
            System.out.println("\n");
        }
    }
}
