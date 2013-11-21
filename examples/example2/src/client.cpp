#include "remote_service.hpp"

#include <remote/session.hpp>
#include <remote/make_tcp_binding.hpp>

#ifdef REMOTE_USER_REGISTER_ARCHIVE
#include <remote/register_archive.ipp>
#endif


int main()
{
	// create data
	person tom("tom", 13);
	person ben("ben", 18);

	std::vector<person> girls;
	girls.push_back(person("jenny", 9));
	girls.push_back(person("amanda", 10));

	boost::shared_ptr<person> eddy = boost::make_shared<person>("eddy", 30);
	boost::shared_ptr<person> andy = boost::make_shared<person>("andy", 38);

	// create session and connect to server
	remote::session session;
	session.start(remote::make_tcp_binding("localhost", 9999));
	session.wait_for_ready();

	// get a shared_ptr of remote_service
	remote_service* service = session.get_raw<remote_service>("test");

	// use the remote pointer
	service->pass_person(tom);
	service->pass_people(girls);

	// pass by reference & pointer.
	service->pass_reference(tom);
	service->pass_pointer(&ben);
	service->pass_shared_ptr(eddy);
	service->pass_weak_ptr(andy);

	// passing null pointer is ok
	service->pass_pointer(0);
	service->pass_shared_ptr(boost::shared_ptr<person>());
	service->pass_weak_ptr(boost::shared_ptr<person>());

	// values changed to referenced objects are updated
	assert(tom.m_age == 14);
	assert(ben.m_age == 19);
	assert(eddy->m_age == 31);
	assert(andy->m_age == 39);

	// release raw pointer
	session.release(service);

	return 0;
}
