#include <stdio.h>
#include <json-c/json.h>

void test_array_del_idx(void);
json_object *make_array(void);

int main()
{
    json_object *new_json_object = NULL;
    json_object *my_string= NULL;
    json_object *my_int= NULL;
    json_object *my_array= NULL;
    json_object *my_object= NULL;
    int iRet = 0;
    size_t i;

    /*json_type_string*/
    my_string = json_object_new_string("\t");
    printf("my_string=[%s]\n", json_object_get_string(my_string));
    printf("my_string.to_string()=[%s]\n", json_object_to_json_string(my_string));
    /*free 释放对象*/
    json_object_put(my_string);

    my_string = json_object_new_string("\\");
    printf("my_string=[%s]\n", json_object_get_string(my_string));
    printf("my_string.to_string()=[%s]\n", json_object_to_json_string(my_string));
    json_object_put(my_string);

    my_string = json_object_new_string("/");
    printf("my_string=[%s]\n", json_object_get_string(my_string));
    printf("my_string.to_string()=[%s]\n", json_object_to_json_string(my_string));
    printf("my_string.to_string(NOSLASHESCAPE)=[%s]\n", json_object_to_json_string_ext(my_string, JSON_C_TO_STRING_NOSLASHESCAPE));
    json_object_put(my_string);

    my_string = json_object_new_string("/foo/bar/baz");
    printf("my_string=[%s]\n", json_object_get_string(my_string));
    printf("my_string.to_string()=[%s]\n", json_object_to_json_string(my_string));
    printf("my_string.to_string(NOSLASHESCAPE)=[%s]\n", json_object_to_json_string_ext(my_string, JSON_C_TO_STRING_NOSLASHESCAPE));
    json_object_put(my_string);

    my_string = json_object_new_string("foo");
    printf("my_string=[%s]\n", json_object_get_string(my_string));
    printf("my_string.to_string()=[%s]\n", json_object_to_json_string(my_string));
    json_object_put(my_string);

    /*json_type_int*/
    my_int = json_object_new_int(9);
    printf("my_int=[%d]\n", json_object_get_int(my_int));
    printf("my_int.to_string()=[%s]\n", json_object_to_json_string(my_int));
    json_object_put(my_int);

    /*json_type_array*/
    my_array = json_object_new_array();
    /*idx 从0开始*/
    json_object_array_add(my_array, json_object_new_int(1));
    json_object_array_add(my_array, json_object_new_int(2));
    json_object_array_add(my_array, json_object_new_int(3));
    json_object_array_put_idx(my_array, 4, json_object_new_int(5));
    printf("my_array=\n");
    for(i=0; i < json_object_array_length(my_array); i++)
    {
        json_object *obj = json_object_array_get_idx(my_array, i);
        printf("\t[%d]=%s\n", (int)i, json_object_to_json_string(obj));
    }
    /*替换 idx 2*/
    json_object_array_put_idx(my_array, 2, json_object_new_string("2"));
    printf("my_array.to_string()=%s\n", json_object_to_json_string(my_array));

    json_object_put(my_array);

    /*测试删除数组元素*/
    test_array_del_idx();

    my_object = json_object_new_object();
    if(!my_object)
    {
        printf("json_object_new_object() err!\n");
        return -1;
    }

    /* 增加 key */
    iRet = json_object_object_add(my_object,"title",my_object);
    if(iRet != -1)
    {
        printf("ERROR: able to successfully add object to itself!iRet = [%d]\n", iRet);
    }

    /*add, del*/
    json_object_object_add(my_object, "abc", json_object_new_int(12));
    json_object_object_add(my_object, "foo", json_object_new_string("bar"));
    json_object_object_add(my_object, "bool0", json_object_new_boolean(0));
    json_object_object_add(my_object, "bool1", json_object_new_boolean(1));

    /*重新设置 abc*/
    json_object_object_del(my_object, "abc");
    json_object_object_add(my_object, "abc", json_object_new_int(456));

    json_object_object_add(my_object, "baz", json_object_new_string("bang"));

    /*get*/
    json_object *get_obj_int = json_object_object_get(my_object,"abc");
    printf("get_obj_int =[%d]\n", json_object_get_int(get_obj_int));

    json_object *get_obj_str = json_object_object_get(my_object,"baz");
    printf("get_obj_str =[%s]\n", json_object_get_string(get_obj_str));

    json_object *baz_obj = json_object_new_string("fark");
    /*
    json-c的内存管理方式, 是基于引用计数的内存树(链), 如果把一个struct json_object 对象a, add到另一个对象b上, 
    就不用显式的释放(json_object_put) a了, 相当于把a挂到了b的对象树上, 释放b的时候, 就会释放a. 
    当a既add到b上, 又add到对象c上时会导致a被释放两次(double free), 这时可以增加a的引用计数(调用函数json_object_get(a)), 
    这时如果先释放b, 后释放c, 当释放b时, 并不会真正的释放a, 而是减少a的引用计数为1, 然后释放c时, 才真正释放a.
    */
    /*引用计数加 1*/
    json_object_get(baz_obj);
    json_object_object_add(my_object, "baz", baz_obj);
    json_object_object_del(my_object, "baz");

    /* baz_obj should still be valid */
    printf("baz_obj.to_string()=%s\n", json_object_to_json_string(baz_obj));
    json_object_put(baz_obj);

    /*json_object_object_add(my_object, "arr", my_array);*/
    printf("my_object=\n");
    json_object_object_foreach(my_object, key, val)
    {
        printf("\t%s: %s\n", key, json_object_to_json_string(val));
    }
    printf("my_object.to_string()=%s\n", json_object_to_json_string(my_object));

    json_object_put(my_object);

    /**/
    new_json_object = json_tokener_parse("{\"animals\":{\"dog\":[{\"name\":\"Rufus\",\"age\":15},{\"name\":\"Marty\",\"age\":13}]}}");

    printf("new_json_object:  %s\n",json_object_to_json_string(new_json_object));

    json_object_put(new_json_object);

    return 0;
}

json_object *make_array()
{
    json_object *my_array;

    my_array = json_object_new_array();
    json_object_array_add(my_array, json_object_new_int(1));
    json_object_array_add(my_array, json_object_new_int(2));
    json_object_array_add(my_array, json_object_new_int(3));
    json_object_array_put_idx(my_array, 4, json_object_new_int(5));
    json_object_array_put_idx(my_array, 3, json_object_new_int(4));
    json_object_array_put_idx(my_array, 6, json_object_new_int(7));

    return my_array;
}

void test_array_del_idx()
{
    int rc;
    size_t ii;
    size_t orig_array_len;
    json_object *my_array;

    my_array = make_array();
    orig_array_len = json_object_array_length(my_array);

    printf("my_array=\n");
    for(ii = 0; ii < json_object_array_length(my_array); ii++)
    {
        json_object *obj = json_object_array_get_idx(my_array, ii);
        printf("\t[%d]=%s\n", (int)ii, json_object_to_json_string(obj));
    }
    printf("my_array.to_string()=%s\n", json_object_to_json_string(my_array));

    for (ii = 0; ii < orig_array_len; ii++)
    {
        rc = json_object_array_del_idx(my_array, 0, 1);
        printf("after del_idx(0,1)=%d, my_array.to_string()=%s\n",
                rc, json_object_to_json_string(my_array));
    }

    /* One more time, with the empty array: */
    rc = json_object_array_del_idx(my_array, 0, 1);
    printf("empty array after del_idx(0,1)=%d, my_array.to_string()=%s\n",
            rc, json_object_to_json_string(my_array));

    json_object_put(my_array);

    /* Delete all array indexes at once */
    my_array = make_array();
    rc = json_object_array_del_idx(my_array, 0, orig_array_len);
    printf("after del_idx(0,%d)=%d, my_array.to_string()=%s\n",
            (int)orig_array_len, rc, json_object_to_json_string(my_array));

    json_object_put(my_array);

    /* Delete *more* than all array indexes at once */
    my_array = make_array();
    rc = json_object_array_del_idx(my_array, 0, orig_array_len + 1);
    printf("after del_idx(0,%d)=%d, my_array.to_string()=%s\n",
            (int)(orig_array_len + 1), rc, json_object_to_json_string(my_array));

    json_object_put(my_array);

    /* Delete some array indexes, then add more */
    my_array = make_array();
    rc = json_object_array_del_idx(my_array, 0, orig_array_len - 1);
    printf("after del_idx(0,%d)=%d, my_array.to_string()=%s\n",
            (int)(orig_array_len - 1), rc, json_object_to_json_string(my_array));

    json_object_array_add(my_array, json_object_new_string("s1"));
    json_object_array_add(my_array, json_object_new_string("s2"));
    json_object_array_add(my_array, json_object_new_string("s3"));

    printf("after adding more entries, my_array.to_string()=%s\n",
            json_object_to_json_string(my_array));
    json_object_put(my_array);
}

/*
gcc example_json.c -I/usr/local/include/json-c -ljson-c
*/

/*输出结果：
$ ./a.out
my_string=[     ]
my_string.to_string()=["\t"]
my_string=[\]
my_string.to_string()=["\\"]
my_string=[/]
my_string.to_string()=["\/"]
my_string.to_string(NOSLASHESCAPE)=["/"]
my_string=[/foo/bar/baz]
my_string.to_string()=["\/foo\/bar\/baz"]
my_string.to_string(NOSLASHESCAPE)=["/foo/bar/baz"]
my_string=[foo]
my_string.to_string()=["foo"]
my_int=[9]
my_int.to_string()=[9]
my_array=
        [0]=1
        [1]=2
        [2]=3
        [3]=null
        [4]=5
my_array.to_string()=[ 1, 2, "2", null, 5 ]
my_array=
        [0]=1
        [1]=2
        [2]=3
        [3]=4
        [4]=5
        [5]=null
        [6]=7
my_array.to_string()=[ 1, 2, 3, 4, 5, null, 7 ]
after del_idx(0,1)=0, my_array.to_string()=[ 2, 3, 4, 5, null, 7 ]
after del_idx(0,1)=0, my_array.to_string()=[ 3, 4, 5, null, 7 ]
after del_idx(0,1)=0, my_array.to_string()=[ 4, 5, null, 7 ]
after del_idx(0,1)=0, my_array.to_string()=[ 5, null, 7 ]
after del_idx(0,1)=0, my_array.to_string()=[ null, 7 ]
after del_idx(0,1)=0, my_array.to_string()=[ 7 ]
after del_idx(0,1)=0, my_array.to_string()=[ ]
empty array after del_idx(0,1)=-1, my_array.to_string()=[ ]
after del_idx(0,7)=0, my_array.to_string()=[ ]
after del_idx(0,8)=-1, my_array.to_string()=[ 1, 2, 3, 4, 5, null, 7 ]
after del_idx(0,6)=0, my_array.to_string()=[ 7 ]
after adding more entries, my_array.to_string()=[ 7, "s1", "s2", "s3" ]
get_obj int =[456]
get_obj string =[bang]
baz_obj.to_string()="fark"
baz_obj.to_string()="fark"
my_object=
        foo: "bar"
        bool0: false
        bool1: true
        abc: 456
my_object.to_string()={ "foo": "bar", "bool0": false, "bool1": true, "abc": 456 }
new_json_object:  { "animals": { "dog": [ { "name": "Rufus", "age": 15 }, { "name": "Marty", "age": 13 } ] } }
*/