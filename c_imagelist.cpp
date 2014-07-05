#include "c_imagelist.h"
#include "console.h"
#include "GUI.h"

bool compare_size( ALLEGRO_BITMAP * first, ALLEGRO_BITMAP * second )
{
	if(al_get_bitmap_width(first) < al_get_bitmap_width(second))
		return true;
	else if(al_get_bitmap_width(first) == al_get_bitmap_width(second))
	{
	if(al_get_bitmap_height(first) < al_get_bitmap_height(second))
		return true;
	}
	return false;
}

size_t c_imagelist::load_image(const char * filename)
{
	size_t num_images = image_list.size();
	for(uint32_t i = 0; i < num_images; i++)
	{
		if (strcmp(filename, image_filenames[i]->c_str()) == 0)
			return i;
	}
	image_list.push_back(load_bitmap_withWarning(filename));
	image_filenames.push_back( new string(filename));
	LogVerbose("Loaded Image: %s\n", filename);
	return image_list.size() - 1;
}

ALLEGRO_BITMAP * c_imagelist::get_image(unsigned int index)
{
	return image_list[index];
}

c_imagelist::c_imagelist()
{
}

void c_imagelist::unload_bitmaps()
{
	for(unsigned int i = 0; i < image_list.size(); i++)
	{
		al_destroy_bitmap(image_list.at(i));
	}
	image_list.clear();
	for(unsigned int i = 0; i < image_filenames.size(); i++)
	{
		delete image_filenames.at(i);
	}

}
//using System;
//using System.Collections.Generic;
//using System.Text;
//using System.Drawing;
//namespace PokeWorldTool.NewObjectSetEditor
//{
//	class PackedSlices
//	{
//		private class SliceHorizCompare : Comparer<ImageSlice>
//		{
//			public override int Compare(ImageSlice x, ImageSlice y)
//			{
//				if (x.Slice.Width < y.Slice.Width)
//				{
//					return 1;
//				}
//				else if (x.Slice.Width > y.Slice.Width)
//				{
//					return -1;
//				}
//				else
//				{
//					return 0;
//				}
//			}
//		}
//		private class SliceVertCompare : Comparer<ImageSlice>
//		{
//			public override int Compare(ImageSlice x, ImageSlice y)
//			{
//				if (x.Slice.Height > y.Slice.Height)
//				{
//					return 1;
//				}
//				else if (x.Slice.Height < y.Slice.Height)
//				{
//					return -1;
//				}
//				else
//				{
//					return 0;
//				}
//			}
//		}
//
//		public class ImageSliceRelocation
//		{
//			public Rectangle Rect;
//			public ImageSlice Slice;
//			public ImageSliceRelocation(Rectangle rect, ImageSlice slice)
//			{
//				Rect = rect;
//				Slice = slice;
//			}
//		}
//		private class PackBox
//		{
//			public Rectangle Box;
//			ImageSlice Slice;
//			List<PackBox> Children;
//			public PackBox(Rectangle box)
//			{
//				Box = box;
//				Children = new List<PackBox>();
//			}
//			public PackBox(ImageSlice slice, int width, int y)
//				: this (new Rectangle (0,y,width,slice.Slice.Height))
//			{
//				PlaceInside(slice);
//			}
//			private void SetSlice(ImageSlice slice)
//			{
//				Slice = slice;
//				int WidthLeft = Box.Width - slice.Slice.Width;
//				int HeightLeft = Box.Height - slice.Slice.Height;
//				if (HeightLeft > 0)
//				{
//					PackBox child = new PackBox(new Rectangle(Box.X, Box.Y + Slice.Slice.Height, Box.Width, Box.Height - slice.Slice.Height));
//					Children.Add(child);
//
//				}
//				if (WidthLeft > 0)
//				{
//					PackBox child = new PackBox(new Rectangle(Box.X + slice.Slice.Width, Box.Y, Box.Width - slice.Slice.Width, slice.Slice.Height));
//					Children.Add(child);
//				}
//			}
//			public void PutSlicesIn(List<ImageSliceRelocation> slices)
//			{
//				if (Slice != null)
//				{
//					Rectangle slice = new Rectangle(Box.X, Box.Y, Slice.Slice.Width, Slice.Slice.Height);
//					slices.Add(new ImageSliceRelocation(slice, Slice));
//					foreach (PackBox child in Children)
//					{
//						child.PutSlicesIn(slices);
//					}
//				}
//			}
//			public bool PlaceInside(ImageSlice slice)
//			{
//				if (Slice == null)
//				{
//					if (Box.Width >= slice.Slice.Width && Box.Height >= slice.Slice.Height)
//					{
//						SetSlice (slice);
//
//						return true;
//					}
//
//					return false;
//				}
//				else
//				{
//
//					foreach (PackBox child in Children)
//					{
//						if (child.PlaceInside (slice))
//						{
//							return true;
//						}
//					}
//					return false;
//				}
//			}
//			public void Draw(Graphics g)
//			{
//				if (Slice != null)
//				{
//					Slice.Draw(g, Box.X, Box.Y);
//					foreach (PackBox child in Children)
//					{
//						child.Draw(g);
//					}
//				}
//			}
//
//		}
//
//
//		public List<ImageSliceRelocation> Slices;
//		public Bitmap Bitmap;
//
//		public static int GetSmallestPowerOf2(int n)
//		{
//			float power = (float)Math.Log(n, 2);
//			power = (float)Math.Ceiling(power);
//			return (int)Math.Pow(2, power);
//		}
//		public int FirstShorter(ImageSlice slice, List<ImageSlice> Slices)
//		{
//			return (Slices.IndexOf(slice) + 1);
//		}
//		public PackedSlices(List<ImageSlice> Unpacked)
//		{
//			List<ImageSlice> HorizSort = new List<ImageSlice>();
//			HorizSort.AddRange(Unpacked);
//			HorizSort.Sort(new SliceHorizCompare());
//			/*List<ImageSlice> VertSort = new List<ImageSlice>();
//			VertSort.AddRange(Unpacked);
//			VertSort.Sort(new SliceHorizCompare());*/
//
//			List<PackBox> PackedBoxes = new List<PackBox>();
//			PackedBoxes.Add(new PackBox(HorizSort[0], GetSmallestPowerOf2(HorizSort[0].Slice.Width), 0));
//
//			ImageSlice box = HorizSort[0];
//			HorizSort.Remove(box);
//
//			while (HorizSort.Count > 0)
//			{
//				ImageSlice slice = HorizSort[0];
//				bool placed = false;
//				foreach (PackBox b in PackedBoxes)
//				{
//					if (b.PlaceInside(slice))
//					{
//						placed = true;
//						HorizSort.Remove(slice);
//						break;
//					}
//				}
//				if (!placed)
//				{
//					PackedBoxes.Add(new PackBox(slice, PackedBoxes[PackedBoxes.Count-1].Box.Width, PackedBoxes[PackedBoxes.Count-1].Box.Bottom));
//					HorizSort.Remove(slice);
//				}
//			}
//
//
//
//			Bitmap = new Bitmap(GetSmallestPowerOf2(PackedBoxes[0].Box.Width), GetSmallestPowerOf2(PackedBoxes[PackedBoxes.Count-1].Box.Bottom));
//			Graphics g = Graphics.FromImage(Bitmap);
//			Slices = new List<ImageSliceRelocation>();
//			foreach (PackBox pb in PackedBoxes)
//			{
//				pb.Draw (g);
//				pb.PutSlicesIn(Slices);
//			}
//
//
//		}
//		public int IndexOfSlice(ImageSlice slice)
//		{
//			foreach (ImageSliceRelocation r in Slices)
//			{
//				if (r.Slice == slice)
//				{
//					return Slices.IndexOf(r);
//				}
//			}
//			return -1;
//		}
//	}
//}
