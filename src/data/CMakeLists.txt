add_lib(${LIB_DATA_NAME}
    AccountsDao.cpp
    ConfigPodsTraits.cpp
    DataDao.cpp
    ItemWrapper.cpp
    TypeTraits.cpp
    data.c
    item.c
    list.c
    page.c
    rbtree.c
)
target_link_libraries(${LIB_DATA_NAME}
    PUBLIC ${LIB_CSV_NAME}
)
