#pragma once
#include <functional> 
#include <memory> 
#include <iostream>

//// Stack Allocator: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//// Code Source: https://www.geeksforgeeks.org/stack-buffer-based-stl-allocator/
//template<class T, std::size_t N, class Allocator = std::allocator<T>> class StackAllocator
//{
//public:
//	// std::allocator_traits<std::allocator<T>>::[value]:
//	typedef typename std::allocator_traits<Allocator>::value_type ValueType;
//	typedef typename std::allocator_traits<Allocator>::pointer Pointer;
//	typedef typename std::allocator_traits<Allocator>::const_pointer ConstPointer;
//	typedef typename Allocator::reference Reference;
//	typedef typename Allocator::const_reference ConstReference;
//	typedef typename std::allocator_traits<Allocator>::size_type SizeType;
//	typedef typename std::allocator_traits<Allocator>::difference_type DifferenceType;
//	typedef typename std::allocator_traits<Allocator>::const_void_pointer ConstVoidPointer;
//
//	typedef Allocator AllocatorType;  // std::allocator<T>
//
//public:
//	explicit StackAllocator(const AllocatorType& alloc = AllocatorType()) : _allocator(alloc), _begin(nullptr), _end(nullptr), _stackPointer(nullptr) {}
//	explicit StackAllocator(Pointer buffer, const AllocatorType& alloc = AllocatorType()) : _allocator(alloc), _begin(buffer), _end(buffer + N), _stackPointer(buffer) {}
//	template<class U> StackAllocator(const StackAllocator<U, N, Allocator>& other) : _allocator(other._allocator), _begin(other._begin), _end(other._end), _stackPointer(other._stackPointer) {}
//
//	constexpr static SizeType Capacity()
//	{
//		return N;
//	}
//
//	Pointer Allocate(SizeType n, ConstVoidPointer hint = ConstVoidPointer())
//	{
//		if (n <= SizeType(std::distance(_stackPointer, _end)))
//		{
//			Pointer result = _stackPointer;
//			_stackPointer += n;
//			return result;
//		}
//
//		return _allocator.allocate(n, hint);
//	}
//
//	void Deallocate(Pointer p, SizeType n)
//	{
//		if (PointerToInternalBuffer(p)) _stackPointer -= n;
//		else _allocator.deallocate(p, n);
//	}
//
//	SizeType max_size() const noexcept
//	{
//		return _allocator.max_size();
//	}
//
//	template <class U, class... Args> void Construct(U* p, Args&&... args)
//	{
//		_allocator.construct(p, std::forward<Args>(args)...);
//	}
//
//	template <class U> void Destroy(U* p)
//	{
//		_allocator.destroy(p);
//	}
//
//	ConstPointer Address(ConstReference x) const noexcept
//	{
//		if (PointerToInternalBuffer(std::addressof(x))) return std::addressof(x);
//		return _allocator.address(x);
//	}
//
//	template <class U>
//	struct Rebind
//	{
//		typedef StackAllocator<U, N, AllocatorType> other;
//	};
//
//	Pointer Buffer() const noexcept
//	{
//		return _begin;
//	}
//
//private:
//	// Check if inputted pointer points to any memory within the allocator:
//	bool PointerToInternalBuffer(ConstPointer p) const
//	{
//		return (!(std::less<ConstPointer>()(p, _begin)) && (std::less<ConstPointer>()(p, _end)));
//	}
//
//	AllocatorType _allocator;
//	Pointer _begin;
//	Pointer _end;
//	Pointer _stackPointer;
//};
//
//template <class T1, std::size_t N, class Allocator, class T2>
//bool operator==(const StackAllocator<T1, N, Allocator>& lhs, const StackAllocator<T2, N, Allocator>& rhs) noexcept
//{
//	return lhs.Buffer() == rhs.Buffer();
//}
//
//template <class T1, std::size_t N, class Allocator, class T2>
//bool operator!=(const StackAllocator<T1, N, Allocator>& lhs, const StackAllocator<T2, N, Allocator>& rhs) noexcept
//{
//	return !(lhs == rhs);
//}
//
//
//// Pool Allocator: ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//// Code Source: http://dmitrysoshnikov.com/compilers/writing-a-pool-allocator/
//struct Element
//{
//	Element* Next;
//};
//
//class PoolAllocator
//{
//public:
//	PoolAllocator(const std::size_t elementSizeBytes) : _elementsPerPool(elementSizeBytes), _allocationPointer(nullptr) {}
//
//	void* Allocate(std::size_t elementSizeBytes)
//	{
//		if (_allocationPointer == nullptr) _allocationPointer = AllocatePool(elementSizeBytes);
//		Element* freeElement = _allocationPointer;
//		_allocationPointer = _allocationPointer->Next;
//		return freeElement;
//	}
//
//	void Deallocate(void* element, std::size_t sizeBytes)
//	{
//		reinterpret_cast<Element*>(element)->Next = _allocationPointer;
//		_allocationPointer = reinterpret_cast<Element*>(element);
//	}
//
//private:
//	Element* AllocatePool(const std::size_t elementSizeBytes)
//	{
//		Element* poolBegin = reinterpret_cast<Element*>(malloc(elementSizeBytes * _elementsPerPool));
//		Element* element = poolBegin;
//		for (std::size_t i = 0; i < _elementsPerPool - 1; ++i)
//		{
//			element->Next = reinterpret_cast<Element*>(reinterpret_cast<char*>(element) + elementSizeBytes);
//			element = element->Next;
//		}
//		element->Next = nullptr;
//		return poolBegin;
//	}
//
//	std::size_t _elementsPerPool;
//	Element* _allocationPointer;
//};
//
//
//struct Object
//{
//	std::uint64_t Data[2];
//	static PoolAllocator Allocator;
//
//	static void* operator new(std::size_t elementSizeBytes)
//	{
//		return Allocator.Allocate(elementSizeBytes);
//	}
//
//	static void operator delete(void* element, std::size_t elementSizeBytes)
//	{
//		return Allocator.Deallocate(element, elementSizeBytes);
//	}
//};