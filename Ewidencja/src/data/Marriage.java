package data;

import java.io.Serializable;
import java.util.Date;

public class Marriage implements Serializable {
    public class InvalidMarriage extends RuntimeException {}
    public class InvalidDataRange extends RuntimeException {}

    private Person _husband;
    private Person _wife;
    private Date _from;
    private Date _to;

    public Marriage(Person p1, Person p2) {
        BaseInfo.Gender p1_gender = p1.getInfo().getGender(),
                        p2_gender = p2.getInfo().getGender();
        if (p1_gender == p2_gender) {
            throw new InvalidMarriage();
        }

        if (p1_gender == BaseInfo.Gender.MALE) {
            this._husband = p1;
            this._wife = p2;
        } else {
            this._husband = p2;
            this._wife = p1;
        }
    }

    public Marriage start(Date begin) {
        this._from = begin;
        return this;
    }

    public Marriage end(Date end) {
        if (this._from.after(end)) {
            throw new InvalidDataRange();
        }

        this._to = end;
        return this;
    }

    public Date getStart() {
        return this._from;
    }

    public Date getEnd() {
        return this._to;
    }

    public Person getHusband() {
        return this._husband;
    }

    public Person getWife() {
        return this._wife;
    }

    public Person getSpouse(Person p) {
        if (p.getInfo().getGender() == BaseInfo.Gender.MALE) {
            return this.getWife();
        }
        return this.getHusband();
    }

    public String getPeriod() {
        String year_from = Integer.toString(this._from.getYear() + 1900);
        String year_to = this._to != null ? Integer.toString(this._to.getYear() + 1900) : "?";
        return "(" + year_from + "-" + year_to + ")";
    }

    public Person haveChild(BaseInfo info) {
        Person child = new Person(info).mother(this._wife).father(this._husband);
        this._husband.addChild(child);
        this._wife.addChild(child);
        return child;
    }
}
